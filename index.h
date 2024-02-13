const char *HTML_CONTENT = R"====(

  <!DOCTYPE html>
<html>
<head>
  <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
</head>
<body>
  <canvas id="ldrChart"></canvas>
  <script>
    var ctx = document.getElementById('ldrChart').getContext('2d');
    var ldrChart = new Chart(ctx, {
      type: 'line',
      data: {
        labels: [],
        datasets: [{
          label: 'LDR Value',
          data: [],
          borderColor: 'rgba(75, 192, 192, 1)',
          borderWidth: 1
        }]
      },
      options: {
        scales: {
          y: {
            beginAtZero: true
          }
        }
      }
    });

    setInterval(function() {
      fetch('/ldr')
        .then(response => response.json())
        .then(data => {
          var now = new Date();
          ldrChart.data.labels.push(now.toLocaleTimeString());
          ldrChart.data.datasets[0].data.push(data.ldr);
          ldrChart.update();
        });
    }, 1000);
  </script>
</body>
</html>

)====";


