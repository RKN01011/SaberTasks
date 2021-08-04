import random
import json


class ListNode(object):
    def __init__(self, new_prev=None, new_data=None, new_next=None, new_rand=None):
        self.prev = new_prev
        self.data = new_data
        self.next = new_next
        self.rand = new_rand

    def __str__(self):
        return str(self.data)

    def __eq__(self, other):
        if other is None:
            return False
        else:
            return self.prev == other.prev and \
                   self.data == other.data and \
                   self.next == other.next and \
                   self.rand == other.rand


class ListRand(object):
    def __init__(self, serialize_data=None):
        self.head = ListNode()
        self.tail = None
        self.count = 0
        if serialize_data:
            arrData = []
            for el in serialize_data:
                arrData.append(
                    {
                        "index": el['index'],
                        "indexRandNode": el['indexRandNode'],
                        "node": self.add(el['data'], False)
                    }
                )
            for el in arrData:
                node = el["node"]
                randNode = arrData[el["indexRandNode"]]["node"]
                node.rand = randNode

    def __len__(self):
        return self.count

    def __iter__(self):
        return ListRandIterator(self)

    def __repr__(self):
        plist = []
        for item in self:
            plist.append(item)
        return str(plist)

    @staticmethod
    def Deserialize(file) -> object:
        data = json.load(file)
        return ListRand(data)

    def get_index_node(self, node):
        result = 0
        while True:
            if node.prev:
                result += 1
                node = node.prev
            else:
                break
        return result

    def Serialize(self, file):
        result = []

        def addSerializeItem(node, index):
            result.append(
                {
                    "index": index,
                    "indexRandNode": self.get_index_node(node.rand),
                    "data": node.data
                }
            )
            if not node.next:
                return
            return addSerializeItem(node.next, index + 1)

        addSerializeItem(self.head, 0)
        file.write(json.dumps(result))

    def add(self, item, need_random=True):
        return self._insert(item, self._create_next_node(), need_random)

    def _insert(self, item, node, need_random):
        node.data = item
        if need_random:
            node.rand = self._getRandNode()
        self.count += 1
        return node

    def _create_next_node(self):
        node = self.head
        for i in range(self.count + 1):
            if node.next:
                node = node.next
                continue
            if i == self.count - 1:
                node.next = ListNode(node, None)
                self.tail = node.next
        return node

    def _getRandNode(self):
        rand_index = 0
        if self.count:
            rand_index = random.randint(1, self.count) - 1
        node = self.head
        for i in range(rand_index):
            if node.next:
                node = node.next
        return node


class ListRandIterator(object):
    def __init__(self, rand_list):
        self._list = rand_list
        self._currentNode = self._list.head

    def __iter__(self):
        return self

    def __next__(self):
        if not self._currentNode:
            raise StopIteration
        else:
            result = self._currentNode.data
            self._currentNode = self._currentNode.next
            return result


def main():
    rand_list = ListRand([])
    rand_list.add('AA')
    rand_list.add('BB')
    rand_list.add('CC')
    print(rand_list)

    f1 = open('ListRandSerialize.json', 'w')
    rand_list.Serialize(f1)
    f1.close()

    f2 = open('ListRandSerialize.json')
    rand_list2 = ListRand.Deserialize(f2)
    f2.close()
    print(rand_list2)


main()
