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

### Numerical Integration Checks

In this sub-project there are utilities to numerically check the computation of
analytic expressions of various integrals. As an examples I provide a two-dimensional
integration that verifies the integration of 
$\pi \int_0^1 dx \int_0^{1-x} \frac{2}{A} + \frac{c^2(1-v^2)(1-x-y)}{A^{3/2}}$
where
$A = 4v^2xy + c^4(1-x-y)^2 + c^2(1+v^2)(1-x-y)(x+y)$. 


