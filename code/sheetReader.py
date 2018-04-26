import json
import sys
import time
import datetime
import gspread
import OpenSSL
from oauth2client.service_account import ServiceAccountCredentials

#static variable for easy usage
GDOCS_OAUTH_JSON = 'PiTempsensors-98f6c6c6fad3.json'

#static variable for easy usage
GDOCS_SPREADSHEET_NAME = 'Pi1'

#how fast Google spreadsheets will be receiving data from Pi
FREQUENCY_SECONDS = 60

def login_open_sheet(oauth_key_file, spreadsheet):
	try:
		json_key = json.load(open('PiTempsensors-98f6c6c6fad3.json'))
		scope = ['https://spreadsheets.google.com/feeds', 'https://www.googleapis.com/auth/drive']
		#credentials = ServiceAccountCredentials(json_key['client_email'],json_key['private_key'].encode(), scope)
		credentials = ServiceAccountCredentials.from_json_keyfile_name(oauth_key_file, scope)
		gc = gspread.authorize(credentials)
		worksheet = gc.open(spreadsheet).sheet1
		print ('got here')
		return worksheet
	except Exception as ex:
		print('unable to login and obtain spreadsheet. Check OAuth credentials, sheet name, and the sheet is shared with the client_email.')
		print('Google SpreadSheet login failed with error:', ex)
		sys.exit(1)
		

print('Logging sensor measurements to {0} every {1} seconds.'.format(GDOCS_SPREADSHEET_NAME, FREQUENCY_SECONDS))
print('Ctrl-C to exit.')
worksheet = None
while True:

	if worksheet is None:
		worksheet = login_open_sheet(GDOCS_OAUTH_JSON, GDOCS_SPREADSHEET_NAME)
		print('opened sheet')
	
	#time.sleep(30)
	f = open("readings.txt", "r")
	theInts = []
	for val in f.readline():
		theInts.append(int(val))
		#tempC = f.read
		#tempF = (tempC * 1.8) + 32
		#humidity = f.read
		#print('got here 2')
		#tempC = 10
		#tempF = 50
		#humidity = 23
	
	temp = (theInts[0] * 1.8) + 32
	theInts.append(temp)	
	#tempC, tempF, humidity = [10, 50, 23]
		
	#try:
	worksheet.append_row((theInts[0], theInts[1], theInts[2]))
	print('got here 3')
	#except:
	#	print('Append error, login retry')
	#	worksheet = None
		#time.sleep(FREQUENCY_SECONDS)
	#	continue
		
print('Wrote a row to {0}'.format(GDOCS_SPREADSHEET_NAME))
	#time.sleep(FREQUENCY_SECONDS)