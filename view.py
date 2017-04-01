#!/usr/bin/python

import sys
import os
import glob

import ctypes

def getName():
	if len(sys.argv) < 2:
		sys.exit()
	elif len(sys.argv) > 2:
		name = ""
		for word in sys.argv:
			if word != sys.argv[0]:
				if word[0] != '*':
					if word[0] != '@':
						if name == "":
							name = name + word
						else:
							name = name + " " + word
		return name
	else:
		return sys.argv[1]

def getStream():
	for stream in sys.argv:
		if stream[0] == '*':
			stream = stream.replace("*", "")
			return stream

def getMode():
	for mode in sys.argv:
		if mode[0] == '@':
			mode = mode.replace("@", "")
			return mode


def keyPressed(stream, name, x, sqllib):

	try:
		with open("postRead.txt", "r") as f:
			postRead = int(f.read())
	except:
		postRead = 1

	if x == '-':
		if postRead != 1:
			postRead = postRead - 1
			with open("postRead.txt", "w") as f:
				f.write(str(postRead))

		sqllib.previousPost(stream, name, postRead)


	elif x == 'a':
		postRead = sqllib.printPost(stream, name)
		with open("postRead.txt", "w") as f:
				f.write(str(postRead))


	elif x == 'c':
		postRead = sqllib.printPost(stream, name)
		with open("postRead.txt", "w") as f:
				f.write(str(postRead))


	elif x == '+':
		postRead2 = sqllib.nextPost(stream, name, postRead)
		with open("postRead.txt", "w") as f:
				f.write(str(postRead2))


	elif x == 'o':
		sqllib.sortByName(stream)


	elif x == 'm':
		sqllib.markAllRead(stream, name)


if __name__ == "__main__":

	name = getName()
	stream = getStream()
	mode = getMode()

	sqllib = ctypes.CDLL('./sqllib.so')

	if stream == None:
		sqllib.connect(name)
		sys.exit()

	keyPressed(stream, name, mode, sqllib)



