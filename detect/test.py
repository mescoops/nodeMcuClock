import socket
import urllib.request

timeout = 0.1
socket.setdefaulttimeout(timeout)

for x in range(255):
	try:
		adr = "192.168.0."+str(x)
		f = urllib.request.urlopen("http://"+adr+"/DETECT")
		got = f.read().decode()
		if "clock-" in ""+got:
			print(adr)
			print(got)
	except:
		o = 1
		#print(adr)
		#print("Nope")
