import gdb.printing

class ListNodePrinter:
    def __init__(self, val):
        self.val = val
        self.max_nodes = 10

    def to_string(self):
        visited = set()
        current = self.val
        items = []

        while current and len(items) < self.max_nodes:
            addr = str(current.address)

            if addr in visited:
                # circles detected
                items.append(f"->{current['val']}")
                break

            visited.add(addr)
            items.append(str(current['val']))
            current = current['next']

        if self.max_nodes < len(items):
            items.append("...")

        return f"[{','.join(items)}]"

def build_pretty_printer():
    pp = gdb.printing.RegexpCollectionPrettyPrinter("leetcode_printer")
    pp.add_printer('ListNode', '^ListNode$', ListNodePrinter)
    return pp

gdb.printing.register_pretty_printer(
    gdb.current_objfile(),
    build_pretty_printer())

