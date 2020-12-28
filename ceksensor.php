<?php
$conn = mysqli_connect("localhost", "root", "", "fuzzy");
$result = mysqli_query($conn, "SELECT * FROM sugeno");
$data = mysqli_fetch_array($result);
$isi = $data ["PWM"];
$xdekat = $data ["posisix_dekat"];
$xsedang = $data ["posisix_sedang"];
$xjauh = $data ["posisix_jauh"];
$ydekat = $data ["posisiy_dekat"];
$ysedang = $data ["posisiy_sedang"];
$yjauh = $data ["posisiy_jauh"];
$dataxb = $data ["dataxb"];
$datayb = $data ["datayb"];

echo "DATA XB = " .$dataxb."<br>DATA YB = " .$datayb. "<br>";

echo "<br> Posisi XB <br>";
echo "[DEKAT] = " .$xdekat. "<br>";
echo "[SEDANG] = " .$xsedang. "<br>";
echo "[JAUH] = " .$xjauh. "<br>";

echo "<br>Posisi YB <br>";
echo "[DEKAT] = " .$ydekat. "<br>";
echo "[SEDANG] = " .$ysedang. "<br>";
echo "[JAUH] = " .$yjauh. "<br>";

echo "<br> NILAI PWM <br>" .$isi. "<br>";

if ($isi <= "100") {
  echo "Lambat";
} elseif ($isi <= "200" && $isi >"100") {
  echo "Sedang";
} elseif ($isi > "200" && $isi <= "250") {
  echo "Cepat";
}
?>