### Safra Determinization ###

An implementation of Saftra's determinization algorithm on omega-automata.

### Building ###

Just run make!

### Usage ###

./safra <input_file> [-o <output_file>] [-c <convert_file>]

Reads in a Buechi automaton in input_file.
Outputs the corresponding Rabin automaton in output_file.
If output_file is not provides, outputs to standard out instead.
The output format is graphviz's dot file format.

If convert_file is provided, the input file is also converted to dot file format and written to convert_file.
