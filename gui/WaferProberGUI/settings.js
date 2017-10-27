var xy_device = "/dev/ttyACM0"

var image_width = 640
var image_height = 480
var margin = 5
// var image_saved_path = "/Users/xju/Documents/2017/RD53/code/labRemote/gui/WaferProberGUI/captured_image/Test_"
var image_saved_path = "/home/pixel/Documents/probing_station/code/labRemote/gui/WaferProberGUI/TEST_"

// image indexes. a closure function.
var add =  (
    function () {
        var counter = 0;
        return function() {return counter += 1;}
    }
)();
