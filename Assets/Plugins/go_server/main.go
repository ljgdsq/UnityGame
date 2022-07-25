package main

import (
	//"bytes"
	//"encoding/json"
	"github.com/gin-gonic/gin"
	"net/http"
)

type Person struct {
	Name string `json:"name"`
	Age int `json:"age"`
}

func main() {



	r := gin.Default()

	r.GET("/GetPlanText", func(c *gin.Context) {
		//输出json结果给调用方

		c.String(http.StatusOK,"hello")
	})
	r.GET("/GetTexture", func(c *gin.Context) {
		//输出json结果给调用方

		c.File("./resources/a.png")
	})
	r.POST("/battle", func(context *gin.Context) {
		context.JSON(200,gin.H{
			"name":"hhhh",
			"id":19,
			"aaa":222,
		})
	})
	r.POST("/PostJson", func(context *gin.Context) {
		var person Person
		context.BindJSON(&person)
		println(person.Name)
		person.Age=22
		context.JSON(200,person)
	})


	r.Run() // listen and serve on 0.0.0.0:8080

}







