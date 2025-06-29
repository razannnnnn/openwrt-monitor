<?php
header("Content-Type: application/json");

// Jalankan speedtest dan ambil output JSON-nya
$output = shell_exec("speedtest-cli --json");

if (!$output) {
    echo json_encode(["error" => "Speedtest gagal dijalankan"]);
    exit;
}

// Konversi dan filter data
$data = json_decode($output, true);

$response = [
    "download_mbps" => round(($data["download"] ?? 0) / 1000000, 2),
    "upload_mbps" => round(($data["upload"] ?? 0) / 1000000, 2),
    "ping_ms" => $data["ping"] ?? 0,
    "server" => $data["server"]["name"] ?? "unknown"
];

echo json_encode($response, JSON_PRETTY_PRINT);
