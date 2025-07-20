<?php
if (isset($argv[1]) && $argv[1] == 'info') {
    // Exiba as informações do sistema
    echo "<p>Sistema: " . php_uname('s') . "</p>";
    echo "<p>Hostname: " . php_uname('n') . "</p>";
    echo "<p>Versão: " . php_uname('v') . "</p>";
    echo "<p>Data/Hora: " . date('Y-m-d H:i:s') . "</p>";
    echo "<p>Uptime: " . shell_exec('uptime') . "</p>";
    exit;
}
?>

<!DOCTYPE html>
<html lang="pt-br">
<head>
    <meta charset="UTF-8">
    <title>Status do Sistema</title>
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
        body {
            margin: 0;
            padding: 20px;
            font-family: "Trebuchet MS", Arial, sans-serif;
            background-color: #f0f0f0;
            background-image: url('data:image/svg+xml,<svg xmlns="http://www.w3.org/2000/svg" width="100" height="100" viewBox="0 0 100 100"><rect width="100" height="100" fill="%23f0f0f0"/><circle cx="50" cy="50" r="1" fill="%23e0e0e0"/></svg>');
            color: #333;
        }
        .container {
            max-width: 800px;
            margin: 0 auto;
            background: #ffffff;
            border: 2px solid #cccccc;
            border-radius: 8px;
            box-shadow: 3px 3px 8px rgba(0,0,0,0.3);
            overflow: hidden;
        }
        .header {
            background: linear-gradient(to bottom, #4a90e2, #357abd);
            color: white;
            padding: 15px 25px;
            border-bottom: 2px solid #2968a3;
            text-align: right;
        }
        .header h1 {
            margin: 0;
            font-size: 24px;
            text-shadow: 1px 1px 2px rgba(0,0,0,0.5);
            font-weight: bold;
        }

        .content {
            padding: 30px;
            text-align: center;
            min-height: 300px;
        }
        #sistema {
            background: #f8f8f8;
            border: 1px solid #ddd;
            border-radius: 4px;
            padding: 20px;
            margin: 20px auto;
            max-width: 500px;
            box-shadow: inset 1px 1px 3px rgba(0,0,0,0.1);
        }
        
        #sistema p {
            margin: 10px 0;
            padding: 8px;
            background: white;
            border-left: 4px solid #4a90e2;
            border-radius: 2px;
            text-align: left;
            font-size: 14px;
        }
        .loading {
            color: #666;
            font-style: italic;
        }
    </style>
    <script>
        function atualizarInfo() {
            fetch('/response.php?info=1')
                .then(response => response.text())
                .then(data => {
                    document.getElementById('sistema').innerHTML = data;
                });
        }
        setInterval(atualizarInfo, 1000);
        window.onload = atualizarInfo;
    </script>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>Status do Sistema</h1>
        </div>
        <div class="content">
            <div id="sistema" class="loading">Carregando informações do sistema...</div>
        </div>
    </div>
</body>
</html>