﻿# coding: utf-8

__author__  = 'Loodoor'
__license__ = 'All right reserved'

import Unamed
from Unamed import upr
import ast, socket, hashlib, sys

###############################################################################
# game stuff                                                                  #
###############################################################################

oprint = print
print = lambda *args, **kw: oprint("PY>", *args, **kw)

class UnamedScript:
    pass

def include(filename):
    f = open("assets/scripts/" + filename)
    fc = f.read()
    c = compile(fc + "\n", filename, "exec")
    glo, loc, t = {}, {}, UnamedScript()
    exec(c, glo, loc)
    t.__dict__.update(glo)
    t.__dict__.update(loc)
    t.__dict__.update({
        "unr_filename": filename,
        "unr_file_length": len(fc),
        "unr_file_content": fc
    })
    f.close()
    return t

sha256 = lambda w: hashlib.sha256(w.encode()).hexdigest()
playerfolder = Unamed.getPlayerFolder()

# CONSTANTS
PRGS_SAVING_PATH = "saves/" + playerfolder + "/progess"
SWITCHS_SAVING_PATH = "saves/" + playerfolder + "/switchs"
VARS_SAVING_PATH = "saves/" + playerfolder + "/vars"
keys_ev_code = ast.literal_eval(open("assets/scripts/data/keysevents.json").read())
PNJkind = {"normal": 0, "special": 1, "system": 2}
BUFFER = 2 ** 10
HOST, PORT = "", 0
IN_TEAM, IN_PC = 0, 1
POCKETS = {
    "Objets": 0,
    "Soins": 1,
    "Balls": 2,
    "Objets rares": 3,
    "Fourre-tout": 4
}

# a table to track down the progress of the player in the adventure
# useful if you want to create an event at a specific moment of the game
#     see events-defining.md (at root top) to know how it is working
_progress = {}
_switchs = {}
_vars = {}

# socket for the network
sock = None

# load progress/switchs...
def load_stuff():
    global _progress
    # progress
    if os.path.exists(PRGS_SAVING_PATH):
        with open(PRGS_SAVING_PATH) as file:
            _progress = ast.literal_eval(file.read())
    # switchs
    if os.path.exists(SWITCHS_SAVING_PATH):
        with open(SWITCHS_SAVING_PATH) as file:
            _switchs = ast.literal_eval(file.read())
    else:
        with open("assets/config/switchs") as file:
            _switchs = ast.literal_eval(file.read())
    # global vars
    if os.path.exists(VARS_SAVING_PATH):
        with open(VARS_SAVING_PATH) as file:
            _vars = ast.literal_eval(file.read())
    else:
        with open("assets/config/vars") as file:
            _vars = ast.literal_eval(file.read())

# save progress/switchs...
def save_stuff():
    with open(PRGS_SAVING_PATH, "w") as file:
        file.write(str(_progress))
    with open(SWITCHS_SAVING_PATH, "w") as file:
        file.write(str(_switchs))
    with open(VARS_SAVING_PATH, "w") as file:
        file.write(str(_vars))

# function to trigger an event from the C++ code
def trigger_event(mid, x, y, triggtype):
    global _progress
    print("trying to trigger an event on map {}, x:{}, y:{}, triggtype:{}".format(mid, x, y, triggtype))
    ev_onmap = _progress.get(mid, {})
    if ev_onmap and ((x, y) in ev_onmap.keys() or triggtype == "autorun"):
        # triggering the autorun events
        if triggtype == "autorun":
            for k, v in ev_onmap.items():
                if v["trigger"] == "autorun":
                    v["triggered"] = True
        # triggering other events' type
        else:
            if ev_onmap[x, y]["trigger"] == triggtype:
                ev_onmap[x, y]["triggered"] = True

# function to init the socket
def netconnect(h, p, proto="TCP"):
    global sock, HOST, PORT
    HOST, PORT = h, p
    if proto == "UDP":
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    elif proto == "TCP":
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((HOST, PORT))

# handle the errors about the network
def nethandle_error(func, *args):
    try:
        return func(*args)
    except socket.gaierror as sge:
        print(sge); return ""
    except ConnectionResetError as cre:
        print("Socket not opened"); return ""

# function to send messages through network
def netsend(message, proto="TCP"):
    global sock, HOST, PORT
    send = lambda m, c: sock.sendto(m, c) if proto == "UDP" else sock.sendall(m)
    nethandle_error(send, str(message).encode(), (HOST, PORT))

# function to receive raw strings from network
def netrecv():
    global sock, BUFFER
    recv = lambda: sock.recv(BUFFER).decode()
    r = nethandle_error(recv)
    return str(r)

# scripts only for the game do not modify/delete them
Unamed.registerScript("runOnceWhenStarting", "doc_extract.py")  # automatic generation of the documentation for the C++/Python binding
Unamed.registerScript("runOnceWhenStarting", "addpnjs.py")      # script loading and putting all the NPC
Unamed.registerScript("runOnceWhenStarting", "textures.py")     # script to load the textures needed
Unamed.registerScript("runWhenUpdatingGame", "ev_checking.py")  # where all the events are checked
Unamed.registerScript("runWhenUpdatingGame", "adventure.py")    # the main adventure, loading automatically all the chapters when they're needed
Unamed.registerScript("runOnceWhenClosing",  "closing.py")      # script run when the game is closing

###############################################################################
# your own scripts here                                                       #
###############################################################################

Unamed.registerScript("runWhenRenderingView", "script.py")

###############################################################################
# functions that need to be execute after everything has been loaded          #
###############################################################################

load_stuff()

print("cwd", os.getcwd())
print("PYTHONHOME", os.environ.get("PYTHONHOME"))
print("HOME", os.environ.get("HOME"))
print("ENCODING", sys.stdout.encoding)
