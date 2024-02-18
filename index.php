<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $username = $_POST['username'];
    // Use the hashed password instead of the original password
    $hashedPassword = $_POST['hashedPassword'];
    
    // Display the entered username and hashed password (with HTML escape)
    echo "<h3>Entered Information:</h3>";
    echo "Username: " . htmlspecialchars($username) . "<br>";
    echo "Hashed Password: " . htmlspecialchars($hashedPassword) . "<br>";
    
    $safe_username = escapeshellarg($username);
    
    // Use the hashed password when calling the C program
    $command = "/var/www/cloud/loginParserC/loginSecurityLib {$safe_username} {$hashedPassword} 2>&1";
    $output = shell_exec($command);
    
    /**
     * 
     * Notice )
     * 
     * The mechanism for checking login success between a C program and a PHP script involves comparing the output string of the C program with a condition in PHP. 
     * When the login is successful in the C program, it prints a string like "==============================Login Successful!!!===================================\n". 
     * Then, the PHP script executes this C program using the shell_exec() function and stores its output in the $output variable.
     * The PHP script examines the contents of this $output variable to check if it contains a specific string. In this case, the presence of the "Login Successful!!!" 
     * string is used to determine the success of the login. If this string exists within $output, it is considered a successful login, 
     * and the user is redirected to the homepage of localhost. Conversely, if this string does not exist, a login failure message is displayed to the user.
     * Through this process, the integration between the login verification logic of the C program and the PHP web interface is implemented, 
     * allowing users to be redirected to the homepage of localhost upon successful login.
     */
    // Check for login success
    if (strpos($output, "Login Successful!!!") !== false) { // "Login Successful!!!"
        header("Location: http://localhost/");
        exit();
    } else {
        echo "<pre>Login Failed!</pre>";
        echo "<pre>$output</pre>";
    }
}
?>

<form method="post" onsubmit="return hashPassword();">
    Username: <input type="text" name="username"><br>
    Password: <input type="password" id="password"><br>
    <!-- Add a hidden field to store the hashed password -->
    <input type="hidden" name="hashedPassword" id="hashedPassword">
    <input type="submit" value="Login">
</form>

<script src="https://cdnjs.cloudflare.com/ajax/libs/crypto-js/4.0.0/crypto-js.min.js"></script>
<script>
function hashPassword() {
    var password = document.getElementById('password').value;
    var hashedPassword = CryptoJS.SHA256(password).toString();
    
    // Set the hashed password in the hidden field
    document.getElementById('hashedPassword').value = hashedPassword;
    
    // Clear the original password field
    document.getElementById('password').value = '';
    return true; // Continue with form submission
}
</script>

