import sys

print("Starting")

while(sys.stdin.readline()[0] == "["):
    print("skipping line")

uci_input = sys.stdin.read()

uci_input = uci_input[:uci_input.find("-")-2]
moves_split = uci_input.split(" ")

output = "{"

for move in moves_split:
    output += "\""
    output += move
    output += "\", "

output += "}"

print(output)

