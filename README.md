# SerialCalculatorStateMachine

Basic floating point calculator implemented using a state machine. It reads input from the specified Serial COM port. The supported operations are +, -, * and /.

## Setup
The Eltima Virtual Serial Port Driver was used to virtualize and interconnect serial COM ports, where you use the first COM port to read the data, and the second one to write to the program.

## Usage
To execute the program, just run the .exe file resulting from compilation, and pass in the COM port to be opened as a parameter. Example:
`.\SerialCalculatorStateMachine.exe COM1`

The format to perform a calculation is the following:
> '('\<First Operand\>\<Operation\>\<Second Operand\>')''='

This format is enforced by a state machine. If you deviate from it, an error message will be sent back and the calculation resets. 
