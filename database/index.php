<!doctype html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=0.98">
    <title>RFID SERVER</title>
</head>
<body>
</body>
</html>

<?php
$conn = new mysqli(127.0.0.1, "rfid_db", "rfid_db", "rfid_db");
$sql = 'SELECT * FROM records';
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    while($row = $result->fetch_assoc()) {
        echo "Date: " . $row["date_entry"]. "  RFID: " . $row["rfid"]. " Num: " . $row["num"] . "<br>";
    }
}
else {
    echo "No Record is found";
}
$conn->close();