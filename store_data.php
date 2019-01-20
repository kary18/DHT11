<?php

// paramètres
$filename = "data.json" ;

// récupération des données Json
$data_json = file_get_contents('php://input');

// vérification du json
$data = json_decode($data_json) ;
if (! $data) {
    http_response_code(415);
    exit();
} elseif (! $data->temperature || ! $data->humidite) {
    http_response_code(400);
    exit();
}

// écriture des données
$op = file_put_contents($filename, $data_json) ;

// vérification de l'écriture des données
if (! $op) {
    http_response_code(500);
    exit();
}

?>
