<!-- YUCK:
# saber

I want to simply use Markdown for the Saber header...
Like this:
# `saber` C++ Header Library of Utility CLasses

But Doxygen renders the backtick-enclosed `saber` as:
<tt>saber</tt>

"<tt>"?! What the?!
So to workaround this, I have to use bogus embedded HTML instead

<h1><code>saber</code><br/>C++ Header Library of Utility Classes</h1>
-->

# `saber`<br/>C++ Header Library of Utility Classes<br/>
A header-only C++ geometry toolkit of generic, templated value types and utilities suitable for numeric, graphics, or geometry code. Types are templated for numeric types and have platform-specific SIMD accelerated implementations.

### C++ version specific features:
- C++17 adds structured binding for geometry classes
- C++20 adds constexpr math for geometry classes

#### External Dependencies (requiring manual install)
- For API documentation generation
  - Doxygen (https://www.doxygen.nl/download.html)
  - Grafviz (https://graphviz.org/download/)
  (Used by Doxygen to generate diagrams/charts/etc.)


<!--
Provide me a summary of the functionality provided by the template classes in this repo that would be suitable as an overview for the repo's README.md

```
Summary — Template functionality overview

Core template types

Point<T, ImplKind>
2D point with X/Y accessors, constructors (default, copy/move, alternate), setters, structured-binding support.
Arithmetic helpers: Translate, Scale and operator overloads (via operators.hpp).
Size<T, ImplKind>
2D size with Width/Height, constructors, setters, Enlarge, Scale, structured-binding and arithmetic helpers.
Rectangle<T, ImplKind>
Origin/Size, X/Y/Width/Height accessors and setters, constructors and copy/move support.
Geometry operations: Translate, Enlarge, Scale.
Set operations: Union, Intersect.
Predicates: IsOverlapping (rectangle-rectangle and rectangle-point), IsEmpty, IsEqual.
Rounding helpers for floating point rectangles: RoundNearest / RoundFloor / RoundCeil.
Utilities

ConvertTo<Target>(Source) conversions between Point and Size.
Free function wrappers for Translate/Scale/Enlarge that mirror member behavior.
Inexact comparison utilities that compare floating values with epsilon proportional to magnitude (used for float/double tolerance tests).
SIMD acceleration (detail/*)

simd_traits and Simd128Traits define platform-specific SIMD types and element counts.
Simd128<T> specializations (int/float/double) provide Load/Store, basic arithmetic (Add/Sub/Mul/Div), compare helpers (IsEq/IsGe/IsLe), DupLo/DupHi, Min/Max, rounding ops, etc.
Architecture-specific specializations implemented for x86 (SSE/IMMINTRIN) and ARM64 (NEON) so Impl2/Impl4 can use vectorized code where available.
Implementation separation

Public API is small header surface (point.hpp, size.hpp, rectangle.hpp, utility.hpp).
Platform- and width-specific implementations live under geometry/detail (impl2/impl4, simd_*.hpp) so the API stays clean while allowing optimized code paths.
Tests

Catch2-based template unit tests cover int/float/double instantiations, constructors, setters, arithmetic, rounding, set operations (Union/Intersect), overlap/empty/is-equal predicates, and inexact comparisons.
Notes for README

The repo provides a compact, well-tested header-only geometry library with an eye toward numeric correctness and SIMD acceleration. It’s suitable as a lightweight dependency for projects needing templated 2D primitives and deterministic numeric utilities.
```
-->