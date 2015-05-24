{ stdenv, mk, pkgconfig }:

stdenv.mkDerivation rec {
  name = "libtags";
  src = ./src;

  buildInputs = [ mk ];
  propagatedBuildInputs = [ pkgconfig ];
  enableParallelBuilding = true;

  buildPhase = "mk -f mkfile.nix";
  installPhase = "mk -f mkfile.nix install";

  meta = {
    description = "A library to read tags";
    maintainers = with stdenv.lib.maintainers; [ ftrvxmtrx ];
    platforms = stdenv.lib.platforms.unix;
    license = stdenv.lib.licenses.mit;
  };
}
