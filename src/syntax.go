package src
var OpenIndexes []int
var CloseIndexes []int
func indexGen(s, sub string) []int {
	var indexes []int
	for index, letter := range s {
		if string(letter) == sub {
			indexes = append(indexes, index)
		}
	}
	return indexes
}
func SyntaxChecker(code string) {
	OpenIndexes=indexGen(code,"[")
	CloseIndexes=indexGen(code,"]")
}
