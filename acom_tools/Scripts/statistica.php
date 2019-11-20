<html>
<head>
  <title>Посещаемость</title>
  <style type="text/css">
      body {
        background-color: #EEE;
        color: #000;
      }
      div {
        font-size: 16px;
      }
      .l
      {
        float: left;
        width: 1px;
        background: #3BF;
        border-right: 1px solid #2AE;
        border-top: 1px solid #2AE;
        position: relative;
        font-size: 7px;
        z-index: 1;
      }
      .r
      {
        position: absolute;
        left: 10px;
        z-index: 2;
        /*width: 800px;
        border-bottom: 1px solid black;  */
      }
      .system
      {
        margin: 25px;
        background: white;
        border-left: 1px solid black;
        border-bottom: 1px solid black;
        width: 800px;
        height: 400px;
        text-align: right;
      }
  </style>
</head>
<body>
<?
  function NDay($d){
    switch(date("w",strtotime($d))){
      case 0: return "ВС";break;
      case 1: return "=)";break;
      case 2: return "ВТ";break;
      case 3: return "СР";break;
      case 4: return "ЧТ";break;
      case 5: return "ПТ";break;
      case 6: return "СБ";break;
    }
  }


  $fo=$_REQUEST['file']; //const value here
  $start = strtotime($_REQUEST['start']);
  $end = strtotime($_REQUEST['end']);
  $first=0;
  $wrote=0;
  $total=0;
  $last=0;
 
  if(!$start){$start="10.04.1998";}
  if(!$end){$end="10.04.2998";}
  if(!$fo){$fo="../nplayers.txt";}
 
  $f=file($fo);
  for($i=0;$i<sizeof($f);$i++){
    for($j=0;$j<strlen($f[$i]);$j++){
      if(($f[$i][$j]=='.')||($f[$i][$j]==' ')||($f[$i][$j]==';')){
        $f[$i][$j]=':';
      }
    }
    list($day[$i], $month[$i], $year[$i], $hour[$i], $minute[$i],
      $second[$i], $players[$i]) = explode(":", $f[$i]);
      if( (mktime($hour[$i],$minute[$i],$second[$i],$month[$i],$day[$i],$year[$i])>=$start) &&
          (mktime($hour[$i-1],$minute[$i-1],$second[$i-1],$month[$i-1],$day[$i-1],$year[$i-1])<=$start) &&
           !($first)){
              $first=$i;
          }
      if( (mktime($hour[$i],$minute[$i],$second[$i],$month[$i],$day[$i],$year[$i])>=$end) &&
          (mktime($hour[$i-1],$minute[$i-1],$second[$i-1],$month[$i-1],$day[$i-1],$year[$i-1])<=$end) &&
           !($last)){
              $last=$i+1;
          }
  }

    if (!($start)){$start=0;}
    if (!($last)){$last=sizeof($f);}
   
    $skip=1;
    $w=800/($last-$first);
    if($w<1){
      $skip=floor(1/$w)+1;
      $w=1;
    }
    $w=floor($w);
   
    echo '<div class="system">';
   
    for($i=$first;$i<$last;$i+=$skip){

      echo "<div valign='bottom' class='l' style='width: ".($w-2)."; top: ".(400-1-$players[$i]*3)."; height: ".($players[$i]*3).";'>".$day[$i].".".$month[$i]."<br>".NDay($year[$i]."-".$month[$i]."-".$day[$i])."<br>".$hour[$i].":".$minute[$i]."<br>".$players[$i]."</div>
          ";
        echo "<div class='r' style='top: ".(414-$players[$i]*3)."px;'>".($players[$i])."</div>
        ";

      $total+=$players[$i];
      $wrote++;
    }
 
 
  echo "</div><br>Среднее количество игроков <b>c ".$start." по ".$end."</b>: ".(floor($total/$wrote));
 
?>
<div style="position: absolute; left: 30px; top: 19px;">/\ игроки</div>
<div style="position: absolute; left: 5px; top: 53px;">120-</div>
<div style="position: absolute; left: 20px; top: 414px;">0-</div>
<div style="position: absolute; left: 827px; top: 416px;">>время</div>
<br><br>
Посмотреть другой период: <br>
  <form action="statistica.php">
    <input type="INPUT" name="start"></input> Начало просмотра<br>
    <input type="INPUT" name="end"></input> Конец просмотра<br>
    <input type="INPUT" name="file"></input> Файл лога<br>        <!--2 delete-->
    <input type="submit" value="Посмотереть"></input>
  </form>
  <br><br>4 Allods.net from kam :)
</body>
</html> 
