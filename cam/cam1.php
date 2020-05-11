<?php
    $received = file_get_contents('php://input');
    $fileToWrite = "cam1.jpg";
    file_put_contents($fileToWrite, $received);
?>

502 acces denied
