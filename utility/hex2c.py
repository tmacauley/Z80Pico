# Utility program to convert Intel Hex file to C source code.
# Requires IntelHex library installed
#   Install with pip install intelhex

import intelhex
import argparse
import sys


class MyIntelHex(intelhex.IntelHex):
    """
    Derived class to make it easy to iterate thru intelhex object.
    Parent class doesn't stop iteration.
    """

    def _bytes(self, start_address=None, thru_address=None):
        if start_address is None:
            start_address = self.minaddr()
        if thru_address is None:
            thru_address = self.maxaddr()
        for address in range(start_address, thru_address+1):
            yield self[address]

    def tocsrc(self, start_address=None, thru_address=None, stride=8):
        print("byte rom[] = {")
        for index, value in enumerate(self._bytes(start_address, thru_address)):
            if not (index % stride):
                print("\t", end="")
            # print(f'{index}: ', end="")
            print(f'0x{value:0>2x}, ', end="")
            if index % stride == stride - 1:
                print("")
        print("}")


class MyParser(argparse.ArgumentParser):
    def error(self, message):
        sys.stderr.write('error: %s\n' % message)
        self.print_help()
        sys.exit(2)


if __name__ == "__main__":

    parser = MyParser(description='Utility program to convert an Intel Hex file to C source code')
    parser.add_argument("hex_file", help="Intel Hex file to read in")
    parser.add_argument("-s", "--start", type=int, default=None,
                        help="Start Address for output. Default=Lowest address in hex File")
    parser.add_argument("-e", "--end", type=int, default=None,
                        help="End Address for output. Default=Top address in hex file")
    parser.add_argument("-p", "--padding", type=int, default=None,
                        help="Byte value for uninitialized data. Default=0xff")
    parser.add_argument("-w", "--width", type=int, default=8,
                        help="Values per line, default=8")
    args = parser.parse_args()

    hex_file = MyIntelHex(args.hex_file)
    if args.padding is not None:
        hex_file.padding = args.padding

    hex_file.tocsrc(args.start, args.end, args.width)
