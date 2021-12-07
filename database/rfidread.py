import json
import mysql.connector
from datetime import datetime

mydb = mysql.connector.connect(
  host=db["127.0.0.1"],
  user=db["rfid_db"],
  passwd=db["rfid_db"],
  database=db["rfid_db"]
)

mycursor = mydb.cursor()
mycursor.execute("CREATE TABLE IF NOT EXISTS records (rfid VARCHAR(64), num VARCHAR(10), date_entry VARCHAR(128)") #Create Database
response = urlopen("http://127.0.0.1") #Get Json data from API
data_json = json.loads(response.read()) #Load json data from HTTP GET
sql = "INSERT INTO records (rdif, num , date_entry) VALUES (%s, %s, %s)"
val = (data_json["RFID"], data_json["Num"], datetime.now())
mycursor.execute(sql, val)
mydb.commit()
