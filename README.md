libtags
=======

A cross-platform library for reading tags, designed for highly constrained environments.

Comparison to id3lib:

```
╭───────────────────┬────────────────────┬────────────────────────╮
│                   │ libtags            │ id3lib                 │
├───────────────────┼────────────────────┼────────────────────────┤
│ ID3v2.4           │ yes                │ no                     │
│ Ogg/Vorbis        │ yes                │ no                     │
│ FLAC              │ yes                │ no                     │
│ replay gain       │ yes                │ no                     │
│ size              │ tiny               │ bloated                │
│ license           │ MIT                │ LGPL                   │
│ written in        │ C                  │ C++                    │
│ memory            │ no allocations     │ allocates memory       │
│ thread safe       │ yes                │ ???                    │
│ speed             │ fast               │ slow                   │
│ tag writing       │ no, not a goal     │ yes                    │
│ Plan 9 compatible │ yes, native        │ no                     │
╰───────────────────┴────────────────────┴────────────────────────╯
```
