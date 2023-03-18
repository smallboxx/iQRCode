// index.js
Page({
    data:{
        src:'cloud://cloud1-0gy2mxyn5f2310d9.636c-cloud1-0gy2mxyn5f2310d9-1316638538/camera.jpg',
        pre_src:'',
        temp_photoid:''
    },
    onLoad(options) {
        this.socket = wx.connectSocket({
          url: 'ws://192.168.1.103:9525',
          success:(res)=>{
              console.log('ss',res)
          }
        })
        this.socket.onOpen(()=>{
            console.log("connect_yes")
        })
        this.socket.onError(() => {
            console.log("connect_fail")
        })
    },
    gotoget:function(){
        let that=this
        wx.chooseMedia({
            count: 1,
            mediaType: ['image'],
            sourceType: ['album', 'camera'],
            maxDuration: 30,
            camera: 'back',
            success(res) {
              console.log(res.tempFiles[0].tempFilePath)
              that.setData({
                  pre_src:res.tempFiles[0].tempFilePath
              })
            }
          })
    },
    upload:function(){
        let that=this
        wx.cloud.uploadFile({
            cloudPath: 'photofile/'+Date.now()+'example.png',
            filePath: that.data.pre_src, // 文件路径
          }).then(res => {
            // get resource ID
            console.log(res.fileID)
            that.setData({
                temp_photoid:res.fileID
            })
            wx.showToast({
                title: '上传成功',
                icon: 'success',
                duration: 2000
              })
              
            wx.cloud.getTempFileURL({
                fileList: [that.data.temp_photoid],
                success: res => {
                  // get temp file URL
                  console.log(res.fileList[0].tempFileURL)
                  this.socket.send({
                      data:res.fileList[0].tempFileURL,
                      success:(sendRES)=>{
                          console.log(sendRES)
                      }
                  })
                },
                fail: err => {
                  // handle error
                }
              })
            setTimeout(function() {
                wx.cloud.deleteFile({
                    fileList: [that.data.temp_photoid]
                  }).then(res => {
                    // handle success
                    console.log('delete_yes')
                  }).catch(error => {
                    // handle error
                  })
            }, 10000);
          }).catch(error => {
            // handle error
          })
    }
})
