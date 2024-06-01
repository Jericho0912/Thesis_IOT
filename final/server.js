const express = require('express');
const { DynamoDBClient } = require('@aws-sdk/client-dynamodb');
const { DynamoDBDocumentClient, ScanCommand } = require('@aws-sdk/lib-dynamodb');
const path = require('path');

const app = express();
const port = 3000;

// Create DynamoDB client
const client = new DynamoDBClient({ region: 'us-east-1' }); // Replace with your region
const ddbDocClient = DynamoDBDocumentClient.from(client);

// Serve static files from the "public" directory
app.use(express.static(path.join(__dirname, 'public')));

// Endpoint to fetch data from DynamoDB
app.get('/data', async (req, res) => {
    try {
        const command = new ScanCommand({ TableName: 'finaldb' }); // Replace with your table name
        const results = await ddbDocClient.send(command);
        console.log('DynamoDB results:', results); // Log results to verify the format
        res.json(results.Items || []);
    } catch (err) {
        console.error('Error fetching data:', err);
        res.status(500).json({ error: 'Failed to fetch data from DynamoDB' });
    }
});

app.listen(port, () => {
    console.log(`Server is running on http://localhost:${port}`);
});
