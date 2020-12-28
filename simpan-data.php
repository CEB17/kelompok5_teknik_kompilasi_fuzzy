<?php
//include 'config.php';
class sugeno{
	 public $link='';
	 function __construct($PWM, $posisix_dekat, $posisix_sedang, $posisix_jauh, $posisiy_dekat, $posisiy_sedang, $posisiy_jauh, $dataxb, $datayb ){
	  $this->connect();
	  $this->updateDB($PWM, $posisix_dekat, $posisix_sedang, $posisix_jauh, $posisiy_dekat, $posisiy_sedang, $posisiy_jauh, $dataxb, $datayb);
	  //$this->storeInDB($PWM, $posisix_dekat);
	 }
	 
	 function connect(){
	  $this->link = mysqli_connect('localhost','root','') or die('Cannot connect to the DB');
	  mysqli_select_db($this->link,'fuzzy') or die('Cannot select the DB');
	 }
	 
	 function storeInDB($PWM, $posisix_dekat, $posisix_sedang, $posisix_jauh, $posisiy_dekat, $posisiy_sedang, $posisiy_jauh, $dataxb, $datayb){
	  $query = "insert into sugeno set PWM='".$PWM."', posisix_dekat='".$posisix_dekat."', posisix_sedang='".$posisix_sedang."', posisix_jauh='".$posisix_jauh."', posisiy_dekat='".$posisiy_dekat."', posisiy_sedang='".$posisiy_sedang."', posisiy_jauh='".$posisiy_jauh."', dataxb='".$dataxb."', datayb='".$datayb."'";
	  $result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
	 }
	 

	function updateDB($PWM, $posisix_dekat, $posisix_sedang, $posisix_jauh, $posisiy_dekat, $posisiy_sedang, $posisiy_jauh, $dataxb, $datayb){
	//$id = $suhu["id"];
	$query = "UPDATE sugeno set PWM='".$PWM."', posisix_dekat='".$posisix_dekat."', posisix_sedang='".$posisix_sedang."', posisix_jauh='".$posisix_jauh."', posisiy_dekat='".$posisiy_dekat."', posisiy_sedang='".$posisiy_sedang."', posisiy_jauh='".$posisiy_jauh."', dataxb='".$dataxb."', datayb='".$datayb."' WHERE id = 1";
	//$query = "UPDATE sugeno set PWM='".$PWM."', posisix_dekat='".$posisix_dekat."'"; //id diubah sesuai id yang sudah ada di database
	$result = mysqli_query($this->link,$query) or die('Errant query:  '.$query);
	}
	 
	}
	if($_GET['PWM'] != '' and $_GET['posisix_dekat'] != '' and $_GET['posisix_sedang'] != '' and $_GET['posisix_jauh'] != '' and $_GET['posisiy_dekat'] != '' and $_GET['posisiy_sedang'] != '' and $_GET['posisiy_jauh'] != '' and $_GET['dataxb'] != '' and $_GET['datayb'] != ''){
	$sugeno=new sugeno($_GET['PWM'],$_GET['posisix_dekat'],$_GET['posisix_sedang'],$_GET['posisix_jauh'],$_GET['posisiy_dekat'],$_GET['posisiy_sedang'],$_GET['posisiy_jauh'],$_GET['dataxb'],$_GET['datayb']);
	}
?>