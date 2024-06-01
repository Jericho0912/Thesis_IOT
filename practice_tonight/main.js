document.addEventListener('DOMContentLoaded', () => {
    const fetchChartData = async () => {
        try {
            const response = await fetch('/data');
            console.log('Response status:', response.status);
            if (!response.ok) throw new Error('Network response was not ok');
            
            const data = await response.json();
            if (!Array.isArray(data)) throw new Error('Data is not an array');
            
            return data;
        } catch (error) {
            console.error('Error fetching chart data:', error);
            return [];
        }
    };

    const prepareChartData = (data) => {
        if (!Array.isArray(data)) {
            console.error('Expected data to be an array');
            return { labels: [], rpmValues: [], voltageValues: [] };
        }

        const labels = data.map(item => new Date(item.timestamps));
        const rpmValues = data.map(item => item.rpm);
        const voltageValues = data.map(item => item.voltage);

        return { labels, rpmValues, voltageValues };
    };

    const createChart = (ctx, labels, data, label, borderColor) => {
        return new Chart(ctx, {
            type: 'line',
            data: {
                labels,
                datasets: [{ label, data, borderColor, fill: false }]
            },
            options: {
                responsive: true,
                scales: {
                    x: {
                        type: 'time',
                        time: { unit: 'minute' },
                        title: { display: true, text: 'Timestamp' }
                    },
                    y: { title: { display: true, text: label } }
                }
            }
        });
    };

    const createCharts = async () => {
        const data = await fetchChartData();
        const { labels, rpmValues, voltageValues } = prepareChartData(data);

        createChart(document.getElementById('rpmChart').getContext('2d'), labels, rpmValues, 'RPM', 'rgb(75, 192, 192)');
        createChart(document.getElementById('voltageChart').getContext('2d'), labels, voltageValues, 'Voltage', 'rgb(255, 99, 132)');
    };

    createCharts();
});

let rpmArr = [], voltageArr = [], upArr = [];
let myChart = Highcharts.chart('container1', {
    title: { text: 'Line chart' },
    subtitle: { text: 'realtime data from wokwi' },
    yAxis: [
        { title: { text: 'RPM' } },
        { title: { text: 'Voltage' }, opposite: true }
    ],
    xAxis: { categories: upArr },
    legend: { layout: 'vertical', align: 'right', verticalAlign: 'middle' },
    plotOptions: { series: { label: { connectorAllowed: false } } },
    series: [
        { name: 'RPM', data: [] },
        { name: 'Voltage', yAxis: 1, data: [] }
    ],
    responsive: {
        rules: [{
            condition: { maxWidth: 500 },
            chartOptions: {
                legend: { layout: 'horizontal', align: 'center', verticalAlign: 'bottom' }
            }
        }]
    }
});

const getWeatherData = () => {
    $.ajax({
        type: "GET",
        url: "https://arduinodaybucket.s3.amazonaws.com/myKey",
        dataType: "json",
        async: false,
        success: (data) => {
            console.log('Data received:', data);
            drawChart(data);
        },
        error: (xhr, status, error) => {
            console.error("JSON error: " + status);
        }
    });
};

const drawChart = (data) => {
    const { rpm, voltage, timestamps } = data;

    console.log('RPM:', rpm);
    console.log('Voltage:', voltage);
    console.log('Timestamps:', timestamps);

    rpmArr.push(Number(rpm));
    voltageArr.push(Number(voltage));
    upArr.push(Number(timestamps));

    console.log('RPM Array:', rpmArr);
    console.log('Voltage Array:', voltageArr);
    console.log('Timestamps Array:', upArr);

    myChart.series[0].setData(rpmArr, true);
    myChart.series[1].setData(voltageArr, true);
};

const intervalTime = 3000; // 3 second interval polling
setInterval(getWeatherData, intervalTime);
