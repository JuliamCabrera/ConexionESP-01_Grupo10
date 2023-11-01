document.addEventListener("DOMContentLoaded", function () {
    const temperatureMercury = document.getElementById("temperature-mercury");
    const temperatureValue = document.getElementById("temperature-value");
    const temperatureInput = document.getElementById("temperature-input");
    const chartCanvas = document.getElementById("temperature-chart");

    let temperatureData = [];
    let chart;

    temperatureInput.addEventListener("input", function () {
        const inputValue = parseFloat(temperatureInput.value);
        updateTemperature(inputValue);
    });

    function updateTemperature(value) {
        if (value >= -40 && value <= 80) {
            const heightPercentage = ((value + 40) / 120) * 100;
            temperatureMercury.style.height = heightPercentage + "%";
            temperatureValue.textContent = value.toFixed(1) + "°C";

            temperatureData.push(value);
            if (temperatureData.length > 100) {
                temperatureData.shift();
            }

            updateChart();
        }
    }

    function updateChart() {
        if (chart) {
            chart.destroy();
        }

        chart = new Chart(chartCanvas, {
            type: "line",
            data: {
                labels: Array.from({ length: temperatureData.length }, (_, i) => i),
                datasets: [
                    {
                        label: null,
                        data: temperatureData,
                        fill: true,
                        borderColor: "rgba(63, 81, 181, 1)",
                        borderWidth: 1,
                    },
                ],
            },
            options: {
                scales: {
                    x: {
                        type: "linear",
                        position: "bottom",
                        title: {
                            display: true,
                            text: "Tiempo",
                        },
                        ticks: {
                            stepSize: 1,
                        },
                    },
                    y: {
                        min: -40,
                        max: 80,
                        title: {
                            display: true,
                            text: "Temperatura (°C)",
                        },
                    },
                },
            },
        });
    }
});
