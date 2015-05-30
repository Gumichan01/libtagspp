/*
 * Have fun reading the following:
 *
 * http://id3.org/id3v2.4.0-structure
 * http://id3.org/id3v2.4.0-frames
 * http://id3.org/d3v2.3.0
 * http://id3.org/id3v2-00
 */
#include "tagspriv.h"

#define synchsafe(d) (((d)[0]&127)<<21 | ((d)[1]&127)<<14 | ((d)[2]&127)<<7 | ((d)[3]&127)<<0)
#define beuint(d) ((d)[0]<<24 | (d)[1]<<16 | (d)[2]<<8 | (d)[3]<<0)

static int
v2cb(Tagctx *ctx, char *k, char *v)
{
	if(memcmp(k, "TAL", 3) == 0 && (k[3] == 0 || k[3] == 'B'))
		ctx->tag(ctx, Talbum, v, 0, 0);
	else if(memcmp(k, "TPE", 3) == 0 && (k[3] == '1' || k[3] == '2'))
		ctx->tag(ctx, Tartist, v, 0, 0);
	else if(memcmp(k, "TP", 2) == 0 && (k[2] == '1' || k[2] == '2'))
		ctx->tag(ctx, Tartist, v, 0, 0);
	else if(strcmp(k, "TIT2") == 0 || strcmp(k, "TT2") == 0)
		ctx->tag(ctx, Ttitle, v, 0, 0);
	else if(memcmp(k, "TYE", 3) == 0 && (k[3] == 0 || k[3] == 'R'))
		ctx->tag(ctx, Tdate, v, 0, 0);
	else if(strcmp(k, "TDRC") == 0)
		ctx->tag(ctx, Tdate, v, 0, 0);
	else if(strcmp(k, "TRK") == 0 || strcmp(k, "TRCK") == 0)
		ctx->tag(ctx, Ttrack, v, 0, 0);
	else if(strcmp(k, "TXXX") == 0 && strncmp(v, "REPLAYGAIN_", 11) == 0){
		int type = -1;
		v += 11;
		if(strncmp(v, "TRACK_", 6) == 0){
			v += 6;
			if(strcmp(v, "GAIN") == 0)
				type = Ttrackgain;
			else if(strcmp(v, "PEAK") == 0)
				type = Ttrackpeak;
		}else if(strncmp(v, "ALBUM_", 6) == 0){
			v += 6;
			if(strcmp(v, "GAIN") == 0)
				type = Talbumgain;
			else if(strcmp(v, "PEAK") == 0)
				type = Talbumpeak;
		}
		if(type >= 0)
			ctx->tag(ctx, type, v+5, 0, 0);
		else
			return 0;
	}else if(memcmp(k, "TCO", 3) == 0 && (k[3] == 0 || k[3] == 'N')){
		for(; v[0]; v++){
			if(v[0] == '(' && v[1] <= '9' && v[1] >= '0'){
				int i = atoi(&v[1]);
				if(i < Numgenre)
					ctx->tag(ctx, Tgenre, id3genres[i], 0, 0);
				for(v++; v[0] && v[0] != ')'; v++);
				v--;
			}else if(v[0] != '(' && v[0] != ')'){
				ctx->tag(ctx, Tgenre, v, 0, 0);
				break;
			}
		}
	}else
		return 0;
	return 1;
}

static int
rva2(Tagctx *ctx, char *tag, int sz)
{
	uchar *b, *end;

	if((b = memchr(tag, 0, sz)) == nil)
		return -1;
	b++;
	for(end = (uchar*)tag+sz; b+4 < end; b += 5){
		int type = b[0];
		float peak;
		float va = (float)(b[1]<<8 | b[2]) / 512.0f;

		if(b[3] == 24){
			peak = (float)(b[4]<<16 | b[5]<<8 | b[6]) / 32768.0f;
			b += 2;
		}else if(b[3] == 16){
			peak = (float)(b[4]<<8 | b[5]) / 32768.0f;
			b += 1;
		}else if(b[3] == 8){
			peak = (float)b[4] / 32768.0f;
		}else
			return -1;

		if(type == 1){ /* master volume */
			char vas[16], peaks[8];
			snprint(vas, sizeof(vas), "%+.5f dB", va);
			snprint(peaks, sizeof(peaks), "%.5f", peak);
			vas[sizeof(vas)-1] = 0;
			peaks[sizeof(peaks)-1] = 0;

			if(strcmp((char*)tag, "track")){
				ctx->tag(ctx, Ttrackgain, vas, 0, 0);
				ctx->tag(ctx, Ttrackpeak, peaks, 0, 0);
			}else if(strcmp((char*)tag, "album")){
				ctx->tag(ctx, Talbumgain, vas, 0, 0);
				ctx->tag(ctx, Talbumpeak, peaks, 0, 0);
			}
			break;
		}
	}
	return 0;
}

static int
resync(uchar *b, int sz)
{
	int i;

	if(sz < 4)
		return sz;
	for(i = 0; i < sz-2; i++){
		if(b[i] == 0xff && b[i+1] == 0x00 && (b[i+2] & 0xe0) == 0xe0){
			memmove(&b[i+1], &b[i+2], sz-i-2);
			sz--;
		}
	}
	return sz;
}

int
tagid3v2(Tagctx *ctx, int *num)
{
	char *tag;
	int sz, exsz, framesz;
	int ver, unsync;
	uchar d[10];

	if(ctx->read(ctx, d, sizeof(d)) != sizeof(d))
		return -1;
	/* "ID3" version[2] flags[1] size[4] */
	if(d[0] != 'I' || d[1] != 'D' || d[2] != '3' ||
		d[3] == 0xff || d[4] == 0xff ||
		d[6] >= 0x80 || d[7] >= 0x80 || d[8] >= 0x80 || d[9] >= 0x80)
		return -1;

	ver = d[3];
	unsync = d[5] & (1<<7);
	sz = synchsafe(&d[6]);

	if(ver == 2 && (d[5] & (1<<6)) != 0) /* compression */
		return -1;

	if(ver > 2){
		if((d[5] & (1<<4)) != 0) /* footer */
			sz -= 10;
		if((d[5] & (1<<6)) != 0){ /* skip extended header */
			if(ctx->read(ctx, d, 4) != 4)
				return -1;
			exsz = (ver >= 3) ? beuint(d) : synchsafe(d);
			if(ctx->seek(ctx, exsz, 1) < 0)
				return -1;
			sz -= exsz;
		}
	}

	framesz = (ver >= 3) ? 10 : 6;
	for(; sz > framesz;){
		int tsz, frameunsync;
		char *b;

		tag = ctx->buf;
		if(ctx->read(ctx, d, framesz) != framesz)
			return -1;
		/* return on padding */
		if(memcmp(d, "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00", framesz) == 0)
			break;
		if(ver >= 3){
			tsz = (ver == 3) ? beuint(&d[4]) : synchsafe(&d[4]);
			if(tsz < 0 || tsz > sz)
				break;
			frameunsync = d[9] & (1<<1);
			d[4] = 0;

			if((d[9] & 0x0c) != 0){ /* compression & encryption */
				ctx->seek(ctx, tsz, 1);
				sz -= tsz;
				continue;
			}
			if(ver == 4 && (d[9] & 1<<0) != 0){ /* skip data length indicator */
				ctx->seek(ctx, 4, 1);
				sz -= 4;
				tsz -= 4;
			}
		}
		else{
			tsz = synchsafe(&d[3]) >> 7;
			if(tsz > sz)
				return -1;
			frameunsync = 0;
			d[3] = 0;
		}
		sz -= framesz;

		if(d[0] != 'T'){ /* skip non-text tags */
			if(memcmp(d, "APIC", 4) == 0){ /* need to skip APIC-specific header */
				/* FIXME cover images */
			}else if(memcmp(d, "RVA2", 4) == 0 && tsz >= 6+5){
				/* replay gain. 6 = "track\0", 5 = other */
				if(ctx->bufsz >= tsz){
					if(ctx->read(ctx, tag, tsz) != tsz)
						return -1;
					if(unsync || frameunsync)
						tsz = resync((uchar*)tag, tsz);
					rva2(ctx, tag, tsz);
					tsz = 0;
				}
			}
			ctx->seek(ctx, tsz, 1);
			sz -= tsz;
		}else{
			sz -= tsz;
			if(ctx->bufsz >= tsz+1){
				/* place the data at the end to make best effort at charset convertion */
				tag = &ctx->buf[ctx->bufsz - tsz - 1];
				if(ctx->read(ctx, tag, tsz) != tsz)
					return -1;
			}else{
				ctx->seek(ctx, tsz, 1);
				continue;
			}

			if(unsync || frameunsync)
				tsz = resync((uchar*)tag, tsz);

			tag[tsz] = 0;
			b = &tag[1];

			switch(tag[0]){
			case 0: /* iso-8859-1 */
				if(iso88591toutf8((uchar*)ctx->buf, ctx->bufsz, (uchar*)b, tsz) > 0)
					*num += v2cb(ctx, (char*)d, ctx->buf);
				break;
			case 1: /* utf-16 */
			case 2:
				if(utf16to8((uchar*)ctx->buf, ctx->bufsz, (uchar*)b, tsz) > 0)
					*num += v2cb(ctx, (char*)d, ctx->buf);
				break;
			case 3: /* utf-8 */
				if(*b)
					*num += v2cb(ctx, (char*)d, b);
				break;
			}
		}
	}

	return 0;
}
