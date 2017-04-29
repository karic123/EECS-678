import os

print( "CLEAR OUTPUT" )
os.system( "rm output/*.txt output/*.out" )

print( "\n\n" )

print( "MAKE" )
os.system( "make" )

print( "\n\n" )

exampleFiles = os.listdir( "examples" )


summary = open( "summary.csv", "w+" )

summary.write( "Fail#,Scheme,Proc,Cores \n" )

count = 0




# Stats

diffs = 0.0
sames = 0.0
totals = 0.0

testLog = []

fails = {
	"proc1" : 0,
	"proc2" : 0,
	"proc3" : 0,
	"c1" : 0,
	"c2" : 0,
	"c4" : 0,
	"fcfs" : 0,
	"rr1" : 0,
	"rr2" : 0,
	"rr4" : 0,
	"sjf" : 0,
	"psjf" : 0,
	"pri" : 0,
	"ppri" : 0
}

passes = {
	"proc1" : 0,
	"proc2" : 0,
	"proc3" : 0,
	"c1" : 0,
	"c2" : 0,
	"c4" : 0,
	"fcfs" : 0,
	"rr1" : 0,
	"rr2" : 0,
	"rr4" : 0,
	"sjf" : 0,
	"psjf" : 0,
	"pri" : 0,
	"ppri" : 0
}

totalItems = {
	"proc1" : 0,
	"proc2" : 0,
	"proc3" : 0,
	"c1" : 0,
	"c2" : 0,
	"c4" : 0,
	"fcfs" : 0,
	"rr1" : 0,
	"rr2" : 0,
	"rr4" : 0,
	"sjf" : 0,
	"psjf" : 0,
	"pri" : 0,
	"ppri" : 0
}


print( "Run examples" )
for exFile in exampleFiles:
	
	if ( ".out" not in exFile ):
		continue
	
	
	
	exFile = exFile[:-4]
	
	# split up
	parts = exFile.split( "-" )
	
	# which proc
	proc = parts[0] + ".csv"
	procB = parts[0]
	
	# how many cores
	coreCount = parts[1][1]
	
	# which scheme
	scheme = parts[2]
	
	# ./simulator -c 1 -s fcfs examples/proc1.csv > output/proc1-c1-fcfs.out
	runString = "./simulator -c " + coreCount + " -s " + scheme + " examples/" + proc + " > output/" + exFile + ".out"
	
	print( runString )
	
	os.system( runString )
	
	# Compare outputs, get final timing diagram
	
	exampleOutfile = open( "examples/" + exFile + ".out" )
	exampleLines = exampleOutfile.readlines()
	exampleOutfile.close()
	
	actualOutfile = open( "output/" + exFile + ".out" )
	actualLines = actualOutfile.readlines()
	actualOutfile.close()
	
	expectedOut = ""
	actualOut = ""
	
	expectedCores = []
	actualCores = []
	
	match = False
	
	printNext = False
	for line in exampleLines:
		if ( "FINAL TIMING DIAGRAM:" in line ):
			printNext = True
		elif printNext:
				expectedOut = expectedOut + "\n" + line
		
		if ( printNext and "Core" in line ):
			expectedCores.append( line )
	
	printNext = False
	for line in actualLines:
		if ( "FINAL TIMING DIAGRAM:" in line ):
			printNext = True
		elif printNext and "DEBUG" not in line:
			actualOut = actualOut + "\n" + line
		
		if ( printNext and "Core" in line ):
			actualCores.append( line )
	
	totals = totals + 1
	totalItems[ "c" + coreCount ] = totalItems[ "c" + coreCount ] + 1
	totalItems[ scheme ] = totalItems[ scheme ] + 1
	totalItems[ procB ] = totalItems[ procB ] + 1
	
	match = ( expectedCores == actualCores )
	
	if ( match ):
		count = count + 1
		
		filename = "output/OK/" + exFile + "_compare.txt"
		sames = sames + 1
		
		passes[ "c" + coreCount ] = passes[ "c" + coreCount ] + 1
		passes[ scheme ] = passes[ scheme ] + 1
		passes[ procB ] = passes[ procB ] + 1
		
		passed = { "Result" : "Passed \t o", "Test  " : count, "Scheme" : scheme, "Cores" : coreCount, "Proc" : procB }
		testLog.append( passed )
		
	else:
		count = count + 1
		
		filename = "output/DIFF/" + exFile + "_compare.txt"
		diffs = diffs + 1
		
		fails[ "c" + coreCount ] = fails[ "c" + coreCount ] + 1
		fails[ scheme ] = fails[ scheme ] + 1
		fails[ procB ] = fails[ procB ] + 1
		
		# summary.write( "Fail#, Scheme, Proc, Cores \n" )
		summary.write( str( count ) + "," + scheme + "," + procB + "," + coreCount + "\n" )
		
		failed = { "Result" : "Failed \t x", "Test  " : count, "Scheme" : scheme, "Cores" : coreCount, "Proc" : procB }
		testLog.append( failed )
	
	
	comparisonFile = open( filename, "w+" )
	
	comparisonFile.write( runString )
	comparisonFile.write( "\n\n" )
	
	comparisonFile.write( "********************************************* \n" )
	comparisonFile.write( "************** EXPECTED OUTPUT ************** \n" )
	comparisonFile.write( "********************************************* \n" )
	comparisonFile.write( expectedOut )
	
	comparisonFile.write( "\n\n" )
	
	comparisonFile.write( "********************************************* \n" )
	comparisonFile.write( "*************** ACTUAL OUTPUT *************** \n" )
	comparisonFile.write( "********************************************* \n" )
	comparisonFile.write( actualOut )
	
	comparisonFile.write( "\n\n" )
	
	
	comparisonFile.close()
	
print( "\n\n" )
print( "RESULT" )
print( "Same outputs:      " + str( int( sames ) ) + "\t(" + str( int( sames/totals*100 ) ) + "%)" )
print( "Different outputs: " + str( int( diffs ) ) + "\t(" + str(int( diffs/totals*100 ) ) + "%)" )
print( "Total:             " + str( int( totals ) ) )

print( "" )

for testdata in testLog:
	print( "" )
	datastr = ""
	for k, v in testdata.items():
		datastr = datastr + k + ": " + str( v ) + "\t"
	print( datastr )

#for k, v in fails.items():
#	print( k + " fails: \t" + str( fails[k] ) + "\t passes: \t" + str( passes[k] ) + "\t totals: \t" + str( totalItems[k] ) )
	


summary.close()
