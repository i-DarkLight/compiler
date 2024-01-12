package src

import (
	"fmt"
	"os"
	"strconv"
	"strings"
)

var OpenIndexes []int
var CloseIndexes []int
var lookahead string
var internalcode string
var pointer int

// var cycleHelper int
// var cycleHelperEnd int
var file *os.File

func indexGen(s, sub string) []int {
	var indexes []int
	for index, letter := range s {
		if string(letter) == sub {
			indexes = append(indexes, index)
		}
	}
	return indexes
}
func match(str string) {
	if lookahead == str && pointer < len(OpenIndexes)-1 {
		pointer += 1
		if strings.Contains(internalcode[OpenIndexes[pointer]+1:CloseIndexes[pointer]], "id") {
			lookahead = "id"
		} else {
			lookahead = internalcode[OpenIndexes[pointer]+1 : CloseIndexes[pointer]]
		}
	} else if pointer == len(OpenIndexes)-1 {
		lookahead = ""
	}
}
func other() {
	var start int = 0
	if lookahead == "id" {
		start = OpenIndexes[pointer]
		match("id")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing var in left side of assign/operation syntax!! Line: " + line + "\n")
		return
	}
	if lookahead == "assign" {
		match("assign")
		value()
	} else {
		operator()
		pp()
	}
	end := OpenIndexes[pointer-1]
	if strings.Contains(internalcode[start:end], "\n") {
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n")
		line := fmt.Sprintf("%v", number)
		file.WriteString("declaration should be on one line!! Line: " + line + "\n")
	}
}
func start() {
	if lookahead == "id" {
		other()
		start()
	} else if lookahead == "cycle" {
		loop()
		start()
	} else if lookahead == "func" {
		function()
		start()
	} else if lookahead == "call" {
		call()
		start()
	} else if lookahead == "if" {
		shart()
		start()
	} else {
		return
	}
}

func multiple() {
	if lookahead == "And" {
		match("And")
		comparisoncond()
	} else if lookahead == "Or" {
		match("Or")
		comparisoncond()
	} else {
		return
	}
}
func assign() {
	if lookahead == "id" {
		match("id")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing var in assign syntax!! Line: " + line + "\n")
		return
	}
	if lookahead == "assign" {
		match("assign")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing 'eq' assign keyword!! Line: " + line + "\n")
		return
	}
	value()
}
func loop() {
	var startloop int = 0
	if lookahead == "cycle" {
		startloop = OpenIndexes[pointer]
		match("cycle")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing cycle keyword for loop syntax!! Line: " + line + "\n")
		return
	}
	if lookahead == "start" {
		match("start")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing 'start' keyword in loop statement!! Line: " + line + "\n")
		return
	}
	assign()
	if lookahead == "end" {
		match("end")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing 'end' keyword in loop statement!! Line: " + line + "\n")
		return
	}
	comparison()
	if lookahead == "pace" {
		match("pace")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing 'pace' keyword in loop statement!! Line: " + line + "\n")
		return
	}
	pace()
	if lookahead == "aculadOpen" {
		match("aculadOpen")
		end := OpenIndexes[pointer-1]
		if strings.Contains(internalcode[startloop:end], "\n") {
			number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n")
			line := fmt.Sprintf("%v", number)
			file.WriteString("loop declaration should be on one line!! Line: " + line + "\n")
		}
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing '{' in loop syntax!! Line: " + line + "\n")
		return
	}
	start()
	if lookahead == "aculadClose" {
		match("aculadClose")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("opened '{' but did not close it!! Line: " + line + "\n")
		return
	}

}
func comparison() {
	if lookahead == "id" {
		match("id")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("use variable name in comparison statements!! Line: " + line + "\n")
		return
	}
	compare()
	pp()
}
func elsecondition() {
	var startelseif int = 0
	if lookahead == "elseif" {
		startelseif = OpenIndexes[pointer]
		match("elseif")
	} else {
		return
	}
	comparisoncond()
	if lookahead == "Colon" {
		match("Colon")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing ':' in else if syntax!! Line: " + line + "\n")
		return
	}
	if lookahead == "aculadOpen" {
		match("aculadOpen")
		end := OpenIndexes[pointer-1]
		if strings.Contains(internalcode[startelseif:end], "\n") {
			number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n")
			line := fmt.Sprintf("%v", number)
			file.WriteString("else if syntax should be on one line!! Line: " + line + "\n")
		}
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing '{' in else if syntax!! Line: " + line + "\n")
		return
	}
	start()
	if lookahead == "aculadClose" {
		match("aculadClose")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("opened '{' but did not close it!! Line: " + line + "\n")
		return
	}

}
func condmirror() {
	var startelse int = 0
	if lookahead == "else" {
		startelse = OpenIndexes[pointer]
		match("else")
	} else {
		return
	}
	if lookahead == "Colon" {
		match("Colon")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing ':' in else syntax!! Line: " + line + "\n")
		return
	}
	if lookahead == "aculadOpen" {
		match("aculadOpen")
		end := OpenIndexes[pointer-1]
		if strings.Contains(internalcode[startelse:end], "\n") {
			number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n")
			line := fmt.Sprintf("%v", number)
			file.WriteString("else syntax should be on one line!! Line: " + line + "\n")
		}
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing '{' in else syntax!! Line: " + line + "\n")
		return
	}
	start()
	if lookahead == "aculadClose" {
		match("aculadClose")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("opened '{' but did not close it!! Line: " + line + "\n")
		return
	}
}
func call() {
	var start int = 0
	if lookahead == "call" {
		start = OpenIndexes[pointer]
		match("call")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing call keyowrd for function call!! Line: " + line + "\n")
		return
	}
	if lookahead == "func" {
		match("func")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing 'func' keyword in call function syntax!! Line: " + line + "\n")
		return
	}
	if lookahead == "id" {
		match("id")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing function name in call function syntax!! Line: " + line + "\n")
		return
	}
	if lookahead == "parantOpen" {
		match("parantOpen")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing '(' to pass parameters!! Line: " + line + "\n")
		return
	}
	parameter()
	if lookahead == "parantClose" {
		match("parantClose")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("opened '(' but did not close it!! Line: " + line + "\n")
		return
	}
	end := OpenIndexes[pointer-1]
	if strings.Contains(internalcode[start:end], "\n") {
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n")
		line := fmt.Sprintf("%v", number)
		file.WriteString("function call should be on one line!! Line: " + line + "\n")
	}
}
func operator() {
	if lookahead == "minus" {
		match("minus")
	} else if lookahead == "plus" {
		match("plus")
	} else if lookahead == "div" {
		match("div")
	} else if lookahead == "multi" {
		match("multi")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing operator!! Line: " + line + "\n")
		return
	}
}
func value() {
	if lookahead == "id" {
		match("id")
	} else if strings.Contains(lookahead, "string") {
		match(lookahead)
	} else if _, check1 := strconv.ParseFloat(lookahead, 64); check1 == nil {
		match(lookahead)
	} else if _, check2 := strconv.Atoi(lookahead); check2 == nil {
		match(lookahead)
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing value as right side of operator!! Line: " + line + "\n")
		return
	}
}
func pace() {
	if lookahead == "id" {
		match("id")
		p()
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("use var name for pace of the loop!! Line: " + line + "\n")
		return
	}
}
func compare() {
	if lookahead == "notequal" {
		match("notequal")
	} else if lookahead == "compare" {
		match("compare")
	} else if lookahead == "minor" {
		match("minor")
	} else if lookahead == "major" {
		match("major")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing operator on this line!! Line: " + line + "\n")
	}
}

func p() {
	if lookahead == "multi" {
		match("multi")
		pp()
	} else if lookahead == "div" {
		match("div")
		pp()
	} else if lookahead == "inc" {
		match("inc")
	} else if lookahead == "dec" {
		match("dec")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("use valid keywords as loop pace!! Line: " + line + "\n")
	}
}
func comparisoncond() {
	if lookahead == "id" {
		match("id")
		compare()
		value()
		multiple()
	} else if _, check := strconv.Atoi(lookahead); check == nil {
		match(lookahead)
		compare()
		value()
		multiple()
	} else if _, check := strconv.ParseFloat(lookahead, 64); check == nil {
		match(lookahead)
		compare()
		value()
		multiple()
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("use correct syntax for condition of if statement!! Line: " + line + "\n")
		return
	}
}
func pp() {
	if lookahead == "id" {
		match("id")
	} else if _, check := strconv.Atoi(lookahead); check == nil {
		match(lookahead)
	} else if _, check := strconv.ParseFloat(lookahead, 64); check == nil {
		match(lookahead)
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing var/number in right side of operator!! Line: " + line + "\n")
	}
}
func parameter() {
	if lookahead == "id" {
		match("id")
	} else if lookahead!="Cama" && lookahead!="parantOpen"{
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("only vars as parameter!! Line: " + line + "\n")
		return
	}
	par()
}
func par() {
	if lookahead == "Cama" {
		match("Cama")
	} else {
		return
	}
	parameter()
}
func function() {
	var startfunction int = 0
	if lookahead == "func" {
		startfunction = OpenIndexes[pointer]
		match("func")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing func keyword for declaring a function!! Line: " + line + "\n")
		return
	}
	if lookahead == "id" {
		match("id")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing function name!! Line: " + line + "\n")
		return
	}
	if lookahead == "parantOpen" {
		match("parantOpen")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing '(' for function parameters!! Line: " + line + "\n")
		return
	}
	parameter()
	if lookahead == "parantClose" {
		match("parantClose")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("opened '(' but did not close it!! Line: " + line + "\n")
		return
	}
	if lookahead == "aculadOpen" {
		match("aculadOpen")
		end := OpenIndexes[pointer-1]
		if strings.Contains(internalcode[startfunction:end], "\n") {
			number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n")
			line := fmt.Sprintf("%v", number)
			file.WriteString("function declaration should be on one line!! Line: " + line + "\n")
		}
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing '{' in function syntax!! Line: " + line + "\n")
		return
	}
	start()
	if lookahead == "aculadClose" {
		match("aculadClose")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("opened '{' but did not close it!! Line: " + line + "\n")
		return
	}
}
func shart() {
	var startif int = 0
	if lookahead == "if" {
		startif = OpenIndexes[pointer]
		match("if")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing cond keyword in if syntax!! Line: " + line + "\n")
		return
	}
	comparisoncond()
	if lookahead == "Colon" {
		match("Colon")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing ':' in  if syntax!! Line: " + line + "\n")
		return
	}
	if lookahead == "aculadOpen" {
		match("aculadOpen")
		end := OpenIndexes[pointer-1]
		if strings.Contains(internalcode[startif:end], "\n") {
			number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n")
			line := fmt.Sprintf("%v", number)
			file.WriteString("if statements should be on one line!! Line: " + line + "\n")
		}
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("missing '{' in if syntax!! Line: " + line + "\n")
		return
	}
	start()
	if lookahead == "aculadClose" {
		match("aculadClose")
	} else {
		match(lookahead)
		number := strings.Count(internalcode[:CloseIndexes[pointer]], "\n") + 1
		line := fmt.Sprintf("%v", number)
		file.WriteString("opened '{' but did not close it!! Line: " + line + "\n")
		return
	}
	elsecondition()
	condmirror()
}
func SyntaxChecker(code string) {
	pointer = 0
	internalcode = code
	errorfile, _ := os.OpenFile("./data/errors.txt", os.O_APPEND|os.O_WRONLY, 0644)
	file = errorfile
	OpenIndexes = indexGen(code, "[")
	CloseIndexes = indexGen(code, "]")
	if strings.Contains(code[OpenIndexes[0]+1:CloseIndexes[0]], "id") {
		lookahead = "id"
	} else {
		lookahead = code[OpenIndexes[0]+1 : CloseIndexes[0]]
	}
	start()
}
