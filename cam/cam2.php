<?php
    $received = file_get_contents('php://input');
    $fileToWrite = "cam2.jpg";
    file_put_contents($fileToWrite, $received);
?>

502 acces dennied
