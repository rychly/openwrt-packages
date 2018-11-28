#!/bin/sh

IFS=$'\n'
PUBLIC=${1:-public}

function makeLink() {
	SPACE=${1}
	shift
	for I in $*; do
		BASENAME=${I##*/}
		echo "${SPACE}<li><a href=\"${I}\">${BASENAME}</a></li>"
	done
}

cd "${PUBLIC}"

cat <<END
<html>
<head>
	<title>???</title>
</head>
<body>
	<h1>???</h1>
	<h2>Distribution</h2>
	<ul>
	</ul>
	<h2>Development</h2>
	<ul>
	</ul>
	<h2>License</h2>
	<pre>$(cat LICENSE | tr -d '<>&')</pre>
</body>
</html>
END
