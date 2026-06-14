# C-based Function Plotter
A function plotter, basically Desmos on a budget, built in C.

## Compile and run
```bash
make
./build/function-plotter "y = [your_latex_function]"
```
Note: Always enclose your LaTeX function in double quotes ("") to prevent the terminal from misinterpreting special characters like \ or spaces.

## Controls
Camera zooming and panning by mouse.
'SPACE': reset camera settings.

The plotter currently supports dynamic grid drawing with an "elastic" zoom effect.

## Supported LaTeX Operators & Functions
The program features a custom two-pass parser that translates standard LaTeX mathematical syntax into tinyexpr-evaluable expressions.

### Basic Arithmetic & Structural

#### Variable: x

#### Basic operators:
* Addtion / Subtraction: +, -

* Multiplication: \cdot, \times

* Fractions: \frac{numerator}{denominator}, \div

#### Algebra & Exponentials

* Square Root: \sqrt{x}

* Euler's Number Power: \exp{x}

* Logarithms: \ln{x} (base e), \log{x} (base 10)

#### Trigonometry

* Standard: \sin{x}, \cos{x}, \tan{x}, \cot{x}

* Inverse: \arcsin{x}, \arccos{x}, \arctan{x}

* Hyperbolic: \sinh{x}, \cosh{x}, \tanh{x}

#### Constants

* Pi: \pi

## Dependencies
* **SDL2**
* **tinyexpr**: **tinyexpr.c** & **tinyexpr.h**