### 图集规范
1. 一个通用图集尺寸2048，如果内容太多unity会自动分成多个texture
2. 多语言图片类型文本建立一个单独的图集，一般来说2048的tex消耗在压缩的情况下仅2M左右
3. 一个预制体一般依赖的图集控制在3个以内，xxxUIAtlas，CommonAtlas，ArtCNAtlas.
4. 每完成一个ui使用工具检查下是否符合规范，atlas引用太多，自身texture冗余等
