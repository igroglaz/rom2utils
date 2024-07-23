# Guide to Run the Script from Ghidra:
# 1. Replace the content of `stack_trace_string` with your actual stack trace.
# 2. Open Ghidra and load your program.
# 3. Go to Window -> Python.
# 4. Copy and paste the following script into the Python interpreter.
# 5. Run the script.

from ghidra.app.decompiler import DecompInterface
from ghidra.util.task import ConsoleTaskMonitor
import re

# Replace the content of `stack_trace_string` with your actual stack trace
stack_trace_string = "BEGIN STACK TRACE: 0x005393ABh <= 0x00509038h <= 0x005090C5h <= 0x004FC3D9h <= 0x0048A811h <= 0x004835FAh <= 0x005DE4EEh <= END STACK TRACE"

# Function to parse stack trace string
def parse_stack_trace(stack_trace):
    pattern = re.compile(r'0x[0-9A-Fa-f]+')
    return [int(match.group(0), 16) for match in pattern.finditer(stack_trace)]

# Parse the stack trace string
stack_trace_addresses = parse_stack_trace(stack_trace_string)

# Initialize the decompiler
decompiler = DecompInterface()
decompiler.openProgram(currentProgram)

# Function to get the function at an address
def get_function(address):
    return getFunctionContaining(toAddr(address))

# Function to get the decompiled code of a function
def get_decompiled_function(function):
    if function is None:
        return None
    try:
        decompilation = decompiler.decompileFunction(function, 60, ConsoleTaskMonitor())
        decompiled_code = decompilation.getDecompiledFunction().getC()
        return decompiled_code
    except Exception as e:
        return "Error decompiling function at 0x{:08X}: {}".format(function.getEntryPoint().getOffset(), str(e))

# List to store results
results = []

# Iterate over addresses and gather function names and their decompiled code
for addr in stack_trace_addresses:
    function = get_function(addr)
    if function:
        function_name = function.getName()
        function_code = get_decompiled_function(function)
        results.append("Function 0x{:08X} - {}:\n{}\n".format(addr, function_name, function_code))
        results.append("\n" + "="*80 + "\n")  # Add separator between functions
    else:
        results.append("Function 0x{:08X} - Unknown Function\n".format(addr))
        results.append("\n" + "="*80 + "\n")  # Add separator for unknown functions

# Print results and save them to a file
file_path = r"C:\Users\n\Desktop\ghidra_rom2.c"
with open(file_path, "w") as file:
    for result in results:
        print(result)
        file.write(result + "\n")

print("Results saved to:", file_path)
