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
	postRead = 0

	if x == '-':
		print 'test'
		f = open("postRead.txt", "r")
		postRead = int(f.read(1))
		f.close()

		if postRead != 1:
			postRead = postRead - 1
			f = open("postRead.txt", "w")
			f.write(str(postRead))
			f.close()

		sqllib.previousPost(stream, name, postRead)

	elif x == 'a':
		postRead = sqllib.printPost(stream, name)

		f = open("postRead.txt", "w")
		f.write(str(postRead))
		f.close()

	elif x == 'c':
		postRead = sqllib.printPost(stream, name)

		f = open("postRead.txt", "w")
		f.write(str(postRead))
		f.close()

	elif x == '+':
		f = open("postRead.txt", "r")
		postRead = int(f.read(1))
		f.close()

		postRead = sqllib.nextPost(stream, name, postRead)
		
		f = open("postRead.txt", "w")
		f.write(str(postRead))
		f.close()

	elif x == 'o':
		print 'test o??'
		sqllib.sortByName(stream)

	elif x == 'm':
		print 'm'
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



