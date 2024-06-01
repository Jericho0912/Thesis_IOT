document.addEventListener('DOMContentLoaded', () => {
    let rpmArr = [], voltageArr = [], upArr = [];
    let rpmChart = Highcharts.chart('container1', {
        title: {
            text: 'RPM Chart'
        },
        subtitle: {
            text: 'Realtime data from S3'
        },
        yAxis: {
            title: {
                text: 'RPM'
            }
        },
        xAxis: {
            categories: upArr,
            title: {
                text: 'Timestamps'
            }
        },
        legend: {
            layout: 'vertical',
            align: 'right',
            verticalAlign: 'middle'
        },
        plotOptions: {
            series: {
                label: {
                    connectorAllowed: false
                }
            }
        },
        series: [{
            name: 'RPM',
            data: []
        }],
        responsive: {
            rules: [{
                condition: {
                    maxWidth: 500
                },
                chartOptions: {
                    legend: {
                        layout: 'horizontal',
                        align: 'center',
                        verticalAlign: 'bottom'
                    }
                }
            }]
        }
    });
    
    let voltageChart = Highcharts.chart('container2', {
        title: {
            text: 'Voltage Chart'
        },
        subtitle: {
            text: 'Realtime data from S3'
        },
        yAxis: {
            title: {
                text: 'Voltage'
            }
        },
        xAxis: {
            categories: upArr,
            title: {
                text: 'Timestamps'
            }
        },
        legend: {
            layout: 'vertical',
            align: 'right',
            verticalAlign: 'middle'
        },
        plotOptions: {
            series: {
                label: {
                    connectorAllowed: false
                }
            }
        },
        series: [{
            name: 'Voltage',
            data: []
        }],
        responsive: {
            rules: [{
                condition: {
                    maxWidth: 500
                },
                chartOptions: {
                    legend: {
                        layout: 'horizontal',
                        align: 'center',
                        verticalAlign: 'bottom'
                    }
                }
            }]
        }
    });

    const getWeatherData = () => {
        $.ajax({
            type: "GET",
            url: "https://arduinodaybucket.s3.amazonaws.com/myKey",
            dataType: "json",
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
        upArr.push(new Date(timestamps).toLocaleTimeString()); // Convert timestamps to readable time

        console.log('RPM Array:', rpmArr);
        console.log('Voltage Array:', voltageArr);
        console.log('Timestamps Array:', upArr);

        rpmChart.series[0].setData(rpmArr, true);
        voltageChart.series[0].setData(voltageArr, true);
        rpmChart.xAxis[0].setCategories(upArr);
        voltageChart.xAxis[0].setCategories(upArr);
    };

    const intervalTime = 3000; // 3-second interval polling
    setInterval(getWeatherData, intervalTime);
});
