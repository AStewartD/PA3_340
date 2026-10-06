const express = require("express");
const mysql = require("mysql2");

const app = express();

app.use(express.json());

const db = mysql.createConnection({
    host: process.env.DB_HOST,
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DB_NAME
});

db.connect((err) => {
    if (err) {
        console.error("Database connection failed:", err);
    } else {
        console.log("Database connection established.");
    }
});

app.post("/api/sensor", (req, res) => {
    const { temperature } = req.body;

    console.log("Received temperature:", temperature);

    const sql = `
        INSERT INTO temperature_data (temperature)
        VALUES (?)
    `;

    db.query(sql, [temperature], (err, result) => {
        if (err) {
            console.error("Database insert failed:", err);
            return res.status(500).json({
                message: "Database insert failed"
            });
        }

        res.json({
            message: "Temperature data received and stored"
        });
    });
});

app.get("/", (req, res) => {
    res.send("Temperature sensor API is running.");
});

const PORT = process.env.PORT || 3000;

app.listen(PORT, "0.0.0.0", () => {
    console.log(`Server running on port ${PORT}`);
});