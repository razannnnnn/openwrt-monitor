<?php
header("Content-Type: application/json");
$board_info = json_decode(shell_exec("ubus call system board"), true);
$temp_raw = trim(@file_get_contents("/sys/class/thermal/thermal_zone0/temp"));
$temperature = is_numeric($temp_raw) ? round($temp_raw / 1000, 1) : "Unavailable";

$rx = $tx = 0;
$netstat = @file_get_contents("/proc/net/dev");
$lines = explode("\n", $netstat);
foreach ($lines as $line) {
    if (strpos($line, "br-lan:") !== false) {
        preg_match('/br-lan:\s*(\d+)\s+\d+\s+\d+\s+\d+\s+\d+\s+\d+\s+\d+\s+\d+\s+(\d+)/', $line, $m);
        $rx = (int)($m[1] ?? 0);
        $tx = (int)($m[2] ?? 0);
        break;
    }
}

$rx_mb = round($rx / 1048576, 2);
$tx_mb = round($tx / 1048576, 2);
$total_bytes = $rx + $tx;
if ($total_bytes >= 1073741824) {
    $bandwidth_used = round($total_bytes / 1073741824, 2) . " GB";
} else {
    $bandwidth_used = round($total_bytes / 1048576, 2) . " MB";
}

$data = [
    "board_name" => $board_info['board_name'] ?? "unknown",
    "model" => $board_info['model'] ?? "unknown",
    "temperature_celsius" => $temperature,
    "br_lan" => [
        "rx_megabytes" => $rx_mb,
        "tx_megabytes" => $tx_mb
    ],
    "bandwidth_used" => $bandwidth_used
];

echo json_encode($data, JSON_PRETTY_PRINT);
