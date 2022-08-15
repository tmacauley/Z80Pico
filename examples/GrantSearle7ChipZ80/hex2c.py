# Utility program to convert Intel Hex file to C cource code.
# Requires IntelHex library installed
#   Install with pip install intelhex

import intelhex
import argparse


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

    def toc(self, start_address=None, thru_address=None, stride=8):
        print("byte rom[] = {")
        for index, value in enumerate(self._bytes(start_address, thru_address)):
            if not (index % stride):
                print("\t", end="")
            # print(f'{index}: ', end="")
            print(f'0x{value:0>2x}, ', end="")
            if index % stride == stride - 1:
                print("")
        print("}")


if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("hex_file", help="Intel Hex file to read in")
    parser.add_argument("-s", "--start", type=int, default=None,
                        help="Start Address for output, default=Min Address")
    parser.add_argument("-e", "--end", type=int, default=None,
                        help="End Address for output, default=Max Address")
    parser.add_argument("-w", "--width", type=int, default=8,
                        help="Values per line, default=8")
    args = parser.parse_args()

    hex_file = MyIntelHex(args.hex_file)

    hex_file.toc(args.start, args.end, args.width)
