import json
import mysql.connector
from urllib.request import urlopen
from datetime import datetime

mydb = mysql.connector.connect(
  host="127.0.0.1",
  user="rfid_db",
  passwd="rfid_db",
  database="rfid_db"
)

mycursor = mydb.cursor()
mycursor.execute("CREATE TABLE IF NOT EXISTS records (count VARCHAR(15), rfid VARCHAR(64), num VARCHAR(10), date_entry VARCHAR(128))") #Create Database
response = urlopen("http://127.0.0.1") #Get Json data from API
data_json = json.loads(response.read()) #Load json data from HTTP GET
sql = "INSERT INTO records (count, rfid, num , date_entry) VALUES (%s, %s, %s, %s)"
val = (data_json["info"]["COUNT"], data_json["info"]["RFID"], data_json["info"]["Num"], datetime.now())
mycursor.execute(sql, val)
mydb.commit()