package src

import (
	"strconv"
	"strings"
)

var OpenIndexes []int
var CloseIndexes []int
var lookahead string
var internalcode string
var pointer int

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
	if lookahead == str {
		pointer++
		lookahead = internalcode[OpenIndexes[pointer]+1 : CloseIndexes[pointer]]
		println(lookahead)
	} else {
		//	println("error")
	}
}
func other() {
	if lookahead == "assign" {
		match("assign")
		value()
	} else {
		operator()
		pp()
	}
}
func start() {
	if lookahead[0:2] == "id" {
		match(lookahead)
		other()
		start()
	} else if lookahead == "cycle" {
		match("cycle")
		loop()
		start()
	} else if lookahead == "func" {
		match("func")
		function()
		start()
	} else if lookahead == "call" {
		match("call")
		call()
		start()
	} else if lookahead == "if" {
		match("if")
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
	}
}
func assign() {
	if lookahead[0:2] == "id" {
		match(lookahead)
		match("assign")
		value()
	}
}
func loop() {
	if lookahead == "start" {
		match("start")
		assign()
		match("end")
		comparison()
		match("pace")
		pace()
		match("aculadOpen")
		start()
		match("aculadClose")

	} else {
		println("error")
	}
}
func comparison() {
	if lookahead[0:2] == "id" {
		match(lookahead)
		compare()
		pp()
	}
}
func elsecondition() {
	if lookahead == "elseif" {
		match("elseif")
		comparisoncond()
		match("Colon")
		match("aculadOpen")
		start()
		match("aculadClose")
	}
}
func condmirror() {
	if lookahead == "else" {
		match("else")
		comparisoncond()
		match("Colon")
		match("aculadOpen")
		start()
		match("aculadClose")
	}
}
func call() {
	if lookahead == "func" {
		match("func")
		match(lookahead)
		match("parantOpen")
		parameter()
		match("parantClose")
	} else {
		println("error")
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
	}
}
func value() {
	if lookahead[0:2] == "id" {
		match(lookahead)
	} else if lookahead[0:6] == "string" {
		match(lookahead)
	} else if _, check1 := strconv.ParseFloat(lookahead, 64); check1 == nil {
		match(lookahead)
	} else if _, check2 := strconv.Atoi(lookahead); check2 == nil {
		match(lookahead)
	} else {
		println("error")
	}
}
func pace() {
	if lookahead[0:2] == "id" {
		match(lookahead)
		p()
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
	}
}
func comparisoncond() {
	if lookahead[0:2] == "id" {
		match(lookahead)
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
		println("error")
	}
}
func pp() {
	if lookahead[0:2] == "id" {
		match(lookahead)
	} else if _, check := strconv.Atoi(lookahead); check == nil {
		match(lookahead)
	} else if _, check := strconv.ParseFloat(lookahead, 64); check == nil {
		match(lookahead)
	} else {
		println("error")
	}
}
func parameter() {
	if lookahead[0:2] == "id" {
		match(lookahead)
		par()
	} else {
		println("error")
	}
}
func par() {
	if lookahead == "Coma" {
		match("Coma")
		parameter()
	}
}
func function() {
	if lookahead[0:2] == "id" {
		match(lookahead)
		match("parantOpen")
		parameter()
		match("parantClose")
		match("aculadOpen")
		start()
		match("aculadClose")
	}
}
func shart() {
	comparisoncond()
	match("Colon")
	match("aculadOpen")
	start()
	match("aculadClose")
	elsecondition()
	condmirror()
}
func format(str string)string{
	var ret string
	for i:=0;i<len(OpenIndexes);i++{
		temp:=str[OpenIndexes[i]+1:CloseIndexes[i]]
		if strings.Contains(temp,"id"){
			ret+="id"
		} else {
			ret+=str[:CloseIndexes[i]]+string(str[CloseIndexes[i]])
		}
	}
	println(ret)
	return ret
}
func SyntaxChecker(code string) string {
	internalcode=format(code)
//	println(internalcode)
	pointer = -1
	OpenIndexes = indexGen(code, "[")
	CloseIndexes = indexGen(code, "]")
	lookahead = code[OpenIndexes[0]+1 : CloseIndexes[0]]
	//start()
	return ""
}
