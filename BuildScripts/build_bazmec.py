import os
from datetime import datetime
from datetime import timezone
import shutil

uatPath = "?" # path to engine RunUAT.bat
projectDir = "?" # path to where this project is located
outDir = "?" # path where to save the build

clientConfig = "?" # Development, Shipping, Test
crashReporter = False # Whether crash reporter is included

def runBuild():

	ts = datetime.now()
	tsUtc = datetime.now(timezone.utc)

	tempHeader = os.path.join(projectDir, "Source", "BazMec", "__buildinfo.h")
	buildInfo = ""

	with open(tempHeader, "w") as header:
		buildInfo = "BazMec.%d.%02d%02d.%02d%02d%02d" % (ts.year, ts.month, ts.day, ts.hour, ts.minute, ts.second)
		print("Building [[%s]]" % buildInfo)
		header.write("#define BZ_BUILDINFO \"%s\"\n" % buildInfo)
		header.write("#define BZ_BUILDTIME \"%s\"" % tsUtc.isoformat())

	try:
		shutil.rmtree(outDir)
	except:
		if (os.path.isdir(outDir)):
			print("Failed to delete %s" % outDir)

	uprojFile = os.path.join(projectDir, "BazMec.uproject")

	extArgs = []

	if crashReporter: extArgs.append("-CrashReporter")

	extArgStr = ""
	for x in extArgs: extArgStr += " %s " % x

	additional="-buildInfo %s" % buildInfo

	uatArgs = "BuildCookRun -project=\"%s\" -noP4 -projectFiles -clientConfig=%s -platform=Win64 -stage -cook -build -pak -prereqs -archive %s -additionalCookerOptions=\"%s\"  -archivedirectory=\"%s\"" % (uprojFile, clientConfig, extArgStr, additional, outDir)
	print("Running [[%s %s]]" % (uatPath, uatArgs), flush=True)

	os.system("%s %s" % (uatPath, uatArgs))
	print("Deleting build info header @ %s" % tempHeader, flush=True)
	os.remove(tempHeader)	
	print("Build done!", flush=True)
