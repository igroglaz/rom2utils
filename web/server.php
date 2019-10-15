<?php
session_start();

// initializing variables
$name = "";
$errors = array(); 

// connect to the database
$db = mysqli_connect('***.***.***.***', 'dblogin', 'dbpassword', 'dbname', '3306');

// REGISTER USER
if (isset($_POST['reg_user'])) {
  // receive all input values from the form
  $name = mysqli_real_escape_string($db, $_POST['name']);
  $password = mysqli_real_escape_string($db, $_POST['password']);
  // form validation: ensure that the form is correctly filled ...
  // by adding (array_push()) corresponding error unto $errors array
  if (empty($name)) { array_push($errors, "name is required"); }
  if (empty($password)) { array_push($errors, "Password is required"); }
  

    // first check the database to make sure 
  // a user does not already exist with the same name and/or email
  $user_check_query = "SELECT * FROM logins WHERE name='$name' LIMIT 1";
  $result = mysqli_query($db, $user_check_query);
  $user = mysqli_fetch_assoc($result);
  
  if ($user) { // if user exists
    if ($user['name'] === $name) {
      array_push($errors, "name already exists");
    }
  }

  // Finally, register user if there are no errors in the form
  if (count($errors) == 0) {
  	$password = sha1($password);//encrypt the password before saving in the database

  	$query = "INSERT INTO logins (name, password) 
  			  VALUES('$name', '$password')";
  	mysqli_query($db, $query);
  	$_SESSION['name'] = $name;
  	$_SESSION['success'] = "You are now logged in";
  	header('location: register.php');
  }
}

// LOGIN USER
if (isset($_POST['login_user'])) {
  $name = mysqli_real_escape_string($db, $_POST['name']);
  $password = mysqli_real_escape_string($db, $_POST['password']);

  if (empty($name)) {
  	array_push($errors, "name is required");
  }
  if (empty($password)) {
  	array_push($errors, "Password is required");
  }

  if (count($errors) == 0) {
  	$password = sha1($password);
  	$query = "SELECT * FROM logins WHERE name='$name' AND password='$password'";
  	$results = mysqli_query($db, $query);
  	if (mysqli_num_rows($results) == 1) {
  	  $_SESSION['name'] = $name;
  	  $_SESSION['success'] = "You are now logged in";
  	  header('location: index.php');
  	}else {
  		array_push($errors, "Wrong name/password combination");
  	}
  }
}

?>