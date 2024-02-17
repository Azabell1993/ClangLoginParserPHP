<?php
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    // 사용자 입력 처리
    $username = $_POST['username']; // escapeshellarg 호출 전 원본 데이터 유지
    $password = $_POST['password']; // escapeshellarg 호출 전 원본 데이터 유지

    // 입력된 사용자 이름과 비밀번호 출력 (HTML escape 처리)
    echo "<h3>입력된 정보:</h3>";
    echo "아이디: " . htmlspecialchars($username) . "<br>";
    echo "비밀번호: " . htmlspecialchars($password) . "<br>";

    // escapeshellarg 함수를 사용하여 쉘 명령어 인자를 안전하게 처리
    $safe_username = escapeshellarg($username);
    $safe_password = escapeshellarg($password);

    // C 프로그램 실행 및 결과 출력
    $command = "/var/www/cloud/loginParserC/loginSecurityLib {$safe_username} {$safe_password} 2>&1";

    $output = shell_exec($command);
    
    // 로그인 성공 여부 확인 (예시)
    if (strpos($output, "로그인 성공!!!") !== false) {
        // 로그인 성공 시 localhost/ 로 리다이렉트
        header("Location: http://localhost/");
        exit();
    } else {
        // 로그인 실패 시 메시지 출력
        echo "<pre>로그인에 실패하셨습니다!</pre>";
        echo "<pre>$output</pre>";
    }
}
?>

<form method="post">
    아이디: <input type="text" name="username"><br>
    비밀번호: <input type="password" name="password"><br>
    <input type="submit" value="로그인">
</form>
