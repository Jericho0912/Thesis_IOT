/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-websocket
 */

const char *HTML_CONTENT = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Dashboard</title>
  <style>
    .container {
      display: grid;
      grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
      grid-gap: 20px;
      padding: 20px;
    }
    .chart-container {
      margin-bottom: 20px;
    }
    canvas {
      width: 100%;
      height: auto;
    }
  </style>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
  <div class="container">
    <div class="chart-container">
      <h2>Energy Chart</h2>
      <canvas id="energyChart"></canvas>
    </div>
    <div class="chart-container">
      <h2>RPM Chart</h2>
      <canvas id="rpmChart"></canvas>
    </div>
    <div class="chart-container">
      <h2>Battery Chart</h2>
      <canvas id="batteryChart"></canvas>
    </div>
  </div>

  <script>
    //(Replace these with your actual data from ESP32)
    const energyData = {
      labels: ['Category 1'],
      datasets: [{  
        label: 'Energy Generated',
        data: [150],
        backgroundColor: 'rgba(255, 99, 132, 0.2)',
        borderColor: 'rgba(255, 99, 132, 1)',
        borderWidth: 1
      }]
    };

    const rpmData = {
      labels: ['Category 1'],
      datasets: [{
        label: 'RPM',
        data: [3000],
        fill: false,
        borderColor: 'rgba(54, 162, 235, 1)',
        borderWidth: 2
      }]
    };

    const batteryData = {
      labels: ['Battery Percentage'],
      datasets: [{
        label: 'Battery Percentage',
        data: [80],
        backgroundColor: 'rgba(255, 206, 86, 0.2)',
        borderColor: 'rgba(255, 206, 86, 1)',
        borderWidth: 1
      }]
    };

    // Chart configurations
    const energyConfig = {
      type: 'bar',
      data: energyData,
      options: {
        responsive: true,
        scales: {
          y: {
            beginAtZero: true
          }
        }
      }
    };

    const rpmConfig = {
      type: 'line',
      data: rpmData,
      options: {
        responsive: true,
        scales: {
          y: {
            beginAtZero: true
          }
        }
      }
    };

    const batteryConfig = {
      type: 'doughnut',
      data: batteryData,
      options: {
        responsive: true,
        plugins: {
          legend: {
            position: 'top',
          },
          title: {
            display: true,
            text: 'Battery Percentage'
          }
        }
      }
    };

    // Create the energy chart
    var energyChart = new Chart(
      document.getElementById('energyChart'),
      energyConfig
    );

    // Create the rpm chart
    var rpmChart = new Chart(
      document.getElementById('rpmChart'),
      rpmConfig
    );

    // Create the battery chart
    var batteryChart = new Chart(
      document.getElementById('batteryChart'),
      batteryConfig
    );
    // Fetch data from the server every 2 seconds
setInterval(function() {
  fetch('/sensor')
    .then(response => response.json())
    .then(data => {
      console.log(data);  // Do something with the data
    });
}, 2000);
  </script>
</body>
</html>

)=====";
