# QFT_tools

Convenient tools to efficiently generate various numbers or expressions for use in QFT.

### Wick Contraction Tool

Useful utilities:
- particles are "half-edges" which have a unique ID, type (arbitrary int), boolean representing if it has already been paired
- vertices are sets of half-edges
- edges are pairs of half-edges
- user can control of which particles can be contracted by allowing to specify different "types" of a particle
- implemented functionality: ordered wick contractions
- implemented functionality: all wick contractions
