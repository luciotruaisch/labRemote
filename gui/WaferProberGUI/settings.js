var xy_device = "/dev/ttyACM0"


// default speed [mm/s]
var speed_x = 2
var speed_y = 2
var speed_z = 0.1

// default increment
var incre_x = 23.6
var incre_y = 28.8534

// size of images
var image_width = 640
var image_height = 480
var margin = 5

// hard coded path that houses captured images
var image_saved_path = "/Users/xju/Documents/2017/RD53/code/labRemote/gui/WaferProberGUI/captured_image/Test_"
// var image_saved_path = "/home/pixel/Documents/probing_station/code/labRemote/gui/WaferProberGUI/TEST_"


var add =  (
    function () {
        var counter = 0;
        return function() {return counter += 1;}
    }
)();
