package main

import (
	//"bytes"
	//"encoding/json"
	"github.com/gin-gonic/gin"
	"net/http"
	"os"
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

	r.GET("/check_version", func(context *gin.Context) {
		context.JSON(200,gin.H{
			"HasNew":true,
			"ForceUpdate":true,
			"NewVer":"v1.0",
			"DownloadURL":"http://127.0.0.1:8080/get_dlc",

		})
	})

	r.POST("/battle", func(context *gin.Context) {
		context.JSON(200,gin.H{
			"name":"hhhh",
			"id":19,
			"aaa":222,
		})
	})

	r.POST("/hello", func(context *gin.Context) {
		context.JSON(200,gin.H{
			"ReplyMsg":"hhhh",
		})
	})
	r.POST("/PostJson", func(context *gin.Context) {
		var person Person
		context.BindJSON(&person)
		println(person.Name)
		person.Age=22
		context.JSON(200,person)
	})

	r.GET("/get_dlc", func(context *gin.Context) {
		ver := context.Query("ver")
		file:=context.Query("file")
	//E:\FileServer\V1.0
		filePath:="E://FileServer/"+ver+"/"+file
		 f,_:=os.Open(filePath)

		stat, _ := f.Stat()
		println("new version " +ver)

		println("file " +filePath)

		context.Header("Content-Type","application/octet-stream")
		context.Header("Content-Disposition","attachment; filename="+file)
		context.Header("Content-Transfer-Encoding","binary")
		context.Header("Content-length", string(stat.Size()))
		context.File(filePath)

	})


	r.Run() // listen and serve on 0.0.0.0:8080

}







