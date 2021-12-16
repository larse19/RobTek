class Node:  # Node has only PARENT_NODE, STATE, DEPTH
    def __init__(self, state, parent=None, depth=0):
        self.STATE = state
        self.PARENT_NODE = parent
        self.DEPTH = depth

    def path(self):  # Create a list of nodes from the root to this node.
        current_node = self
        path = [self]
        while current_node.PARENT_NODE:  # while current node has parent
            current_node = current_node.PARENT_NODE  # make parent the current node
            path.append(current_node)   # add current node to path
        return path

    def display(self):
        print(self)

    def __repr__(self):
        return 'State: ' + str(self.STATE) + ' - Depth: ' + str(self.DEPTH)
    
    '''
    h(n): estimated cost from n to goal (heuristic)
    '''
    def getH(self):
        print(self.STATE[1])
        return self.STATE[1] * 3
    
    def getParrentLetter(self):
        return self.PARENT_NODE.STATE[0]
    
    '''
    g(n): cost so far to reach n (path cost)
    '''
    def getG(self):
        if self.PARENT_NODE == None:
            return 0
        toParent = PATH_COST.get((self.STATE[0], self.getParrentLetter()))
        print(self.STATE[0], self.getParrentLetter(), toParent)
        return toParent + self.PARENT_NODE.getG()

    '''
    f(n) = g(n) + h(n) // f(n) is to estimated total cost of the path through node n to the goal
    '''
    def getF(self):
        return self.getH() + self.getG()

'''
Locates the node with the lowest total path cost
'''
def a_star_node(queue): 
    lowest = queue[0]
    for i in queue:
        if i.getF() <= lowest.getF():
            lowest = i
            queue.remove(i)
    return lowest

'''
A* search, search the tree for goal state and return the path from inital state to goal
'''
def a_star_search():
    fringe = []
    initial_node = Node(INITIAL_STATE)
    fringe = INSERT(initial_node, fringe)
    while fringe is not None:
        node = a_star_node(fringe)
        if node.STATE == GOAL_STATE[0]: #or node.STATE == GOAL_STATE[1]: # Two goal states
            return node.path()
        children = EXPAND(node)
        fringe = INSERT_ALL(children, fringe)
        print("fringe: {}".format(fringe))


'''
Expands node and gets the successors (children) of that node.
Return list of the successor nodes.
'''
def EXPAND(node):
    successors = []
    children = successor_fn(node.STATE)
    for child in children:
        s = Node(node)  # create node for each in state list
        s.STATE = child  # e.g. result = 'F' then 'G' from list ['F', 'G']
        s.PARENT_NODE = node
        s.DEPTH = node.DEPTH + 1
        successors = INSERT(s, successors)
    return successors


'''
Insert node in to the queue (fringe).
'''
def INSERT(node, queue):
    queue.append(node)
    return queue

'''
Insert list of nodes into the fringe
'''
def INSERT_ALL(list, queue):
    queue.extend(list)
    return queue

'''
Removes and returns the first element from fringe
'''
def REMOVE_FIRST(queue):
    return queue.pop(0)

'''
Successor function, mapping the nodes to its successors
'''
def successor_fn(state):  # Lookup list of successor states
    return STATE_SPACE[state]  # successor_fn( 'C' ) returns ['F', 'G']

def distance_manhatten():
    global stateA, stateB, stateC, stateD, stateE, stateF, stateG, stateH, stateI, stateJ, stateK, stateL, stateM, stateN, stateO, stateP, stateQ, stateR, stateS
    for i in range(len(STATE_POSITION)):
        p1 = STATE_POSITION[i]
        p2 = STATE_POSITION[2]

        #print(p1[0], p2, i)

        dist = (abs(p1[1] - p2[1]) + abs(p1[2] - p2[2]))
        #print("Distance between point:", p1[0], "and point:", p2[0], "is =", dist)

        for i, item in enumerate(STATES):
            itemList = list(item)
            if itemList[0] == p1[0]:
                itemList[1] = dist
                item = tuple(itemList)
            STATES[i] = item
        
    stateA = STATES[0]
    stateB = STATES[1]
    stateC = STATES[2]
    stateD = STATES[3]
    stateE = STATES[4]
    stateF = STATES[5]
    stateG = STATES[6]
    stateH = STATES[7]
    stateI = STATES[8]
    stateJ = STATES[9]
    stateK = STATES[10]
    stateL = STATES[11]
    stateM = STATES[12]
    stateN = STATES[13]
    stateO = STATES[14]
    stateP = STATES[15]
    stateQ = STATES[16]
    stateR = STATES[17] 
    stateS = STATES[18]
    setInitialState(stateK)
    setGoalState(stateC)
    setStateSpace()

def setInitialState(state):
    global INITIAL_STATE
    INITIAL_STATE = state

def setGoalState(state):
    global GOAL_STATE
    GOAL_STATE = state

def setStateSpace():
    global STATE_SPACE
    dict = { stateA: [stateB, stateI],
            stateB: [stateA, stateC, stateF],
            stateC: [stateB, stateD],
            stateD: [stateC, stateE],
            stateE: [stateD, stateL],
            stateF: [stateB, stateG],
            stateG: [stateF],
            stateH: [stateK],
            stateI: [stateA, stateJ, stateO],
            stateJ: [stateI, stateK, stateM],
            stateK: [stateJ, stateH, stateL],
            stateL: [stateE, stateK, stateN],
            stateM: [stateJ, stateN, stateP],
            stateN: [stateM, stateL, stateS],
            stateO: [stateI, stateP, stateQ],
            stateP: [stateO, stateM],
            stateQ: [stateO, stateR],
            stateR: [stateQ, stateS],
            stateS: [stateR, stateN]}
    STATE_SPACE = dict

#   0   1   2   3
#
#0  A   B   C
#
#1          D   E
#
#2      F   G
#
#3          H
#
#4  I   J   K   L
#
#5      M       N
#
#6  O   P
#
#7  Q   R       S

STATE_POSITION = [("A", 0, 0), ("B", 0, 1), ("C", 0, 2), 
("D", 1, 2), ("E", 1, 3), 
("F", 2, 1), ("G", 2, 2), 
("H", 3, 2), 
("I", 4, 0), ("J", 4, 1), ("K", 4, 2), ("L", 4, 3),
("M", 5, 1), ("N", 5, 3), 
("O", 6, 0), ("P", 6, 1), 
("Q", 7, 0), ("R", 7, 1), ("S", 7, 3)]

#TODO
#Find a better way to dinamically set the H value
STATES = [("A", int), ("B", int), ("C", int), ("D", int), ("E", int), 
("F", int), ("G", int), ("H", int), ("I", int), ("J", int), ("K", int), 
("L", int), ("M", int), ("N", int), ("O", int), ("P", int), ("Q", int), ("R", int), ("S", int)]

stateA = ()
stateB = ()
stateC = ()
stateD = ()
stateE = ()
stateF = ()
stateG = ()
stateH = ()
stateI = ()
stateJ = ()
stateK = ()
stateL = ()
stateM = ()
stateN = ()
stateO = ()
stateP = ()
stateQ = ()
stateR = ()
stateS = ()

INITIAL_STATE = ('K', 4)
GOAL_STATE = [('C', 0)]
STATE_SPACE = {('A', 2): [('B', 1), ('I', 6)], ('B', 1): [('A', 2), ('C', 0), ('F', 3)], ('C', 0): [('B', 1), ('D', 1)], ('D', 1): [('C', 0), ('E', 2)], ('E', 2): [('D', 1), ('L', 5)], ('F', 3): [('B', 1), ('G', 2)], ('G', 2): [('F', 3)], ('H', 3): [('K', 4)], ('I', 6): [('A', 2), ('J', 5), ('O', 8)], ('J', 5): [('I', 6), ('K', 4), ('M', 6)], ('K', 4): [('J', 5), ('H', 3), ('L', 5)], ('L', 5): [('E', 2), ('K', 4), ('N', 6)], ('M', 6): [('J', 5), ('N', 6), ('P', 7)], ('N', 6): [('M', 6), ('L', 5), ('S', 8)], ('O', 8): [('I', 6), ('P', 7), ('Q', 9)], ('P', 7): [('O', 8), ('M', 6)], ('Q', 9): [('O', 8), ('R', 8)], ('R', 8): [('Q', 9), ('S', 8)], ('S', 8): [('R', 8), ('N', 6)]}

'''
Path cost from child to parent node
'''
PATH_COST = { ("B", "A"): 1,
              ("A", "B"): 1,
              #AI
              ("I", "A"): 4,
              ("A", "I"): 4,
              #CB
              ("C", "B"): 1,
              ("B", "C"): 1,
              #FB
              ("B", "F"): 2,
              ("F", "B"): 2,
              #FG
              ("G", "F"): 1,
              ("F", "G"): 1,
              #C-D
              ("D", "C"): 1,
              ("C", "D"): 1,
              #DE
              ("D", "E"): 1,
              ("E", "D"): 1,
              #EL
              ("L", "E"): 3,
              ("E", "L"): 3,
              #HK
              ("K", "H"): 1,
              ("H", "K"): 1,
              #IJ
              ("J", "I"): 1,
              ("I", "J"): 1,
              #JK
              ("K", "J"): 1,
              ("J", "K"): 1,
              #KL
              ("L", "K"): 1,
              ("K", "L"): 1,
              #NL
              ("L", "N"): 1,
              ("N", "L"): 1,
              #JM
              ("M", "J"): 1,
              ("J", "M"): 1,
              #MN
              ("N", "M"): 2,
              ("M", "N"): 2,
              #OI
              ("I", "O"): 2,
              ("O", "I"): 2,
              #MP
              ("P", "M"): 1,
              ("M", "P"): 1,
              #OP
              ("P", "O"): 1,
              ("O", "P"): 1,
              #QO
              ("O", "Q"): 1,
              ("Q", "O"): 1,
              #QR
              ("Q", "R"): 1,
              ("R", "Q"): 1,
              #RS
              ("S", "R"): 2,
              ("R", "S"): 2,
              #NS
              ("S", "N"): 2,
              ("N", "S"): 2}


'''
Run tree search and display the nodes in the path to goal node
'''
def run():
    # distance_manhatten()
    # print(INITIAL_STATE)
    # print(GOAL_STATE)
    # print(STATE_SPACE)

    path = a_star_search()
    print('Solution path:')
    for node in path:
       node.display()

if __name__ == '__main__':
    run()
