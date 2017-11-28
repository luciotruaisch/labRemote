var xy_device = "/dev/ttyACM0"


// default speed [mm/s]
var speed_x = 2
var speed_y = 2
var speed_z = 0.1

// default increment (stepping interval)
var incre_x = 28.7998
var incre_y = 23.6


// size of images
var image_width = 640
var image_height = 480
var margin = 5

// hard coded path that houses captured images
// var image_saved_path = "/Users/xju/Documents/2017/RD53/code/labRemote/gui/WaferProberGUI/captured_image/Test_"
var image_saved_path = "/home/pixel/Documents/probing_station/code/labRemote/gui/WaferProberGUI/captured_image/TEST_"


var add =  (
    function () {
        var counter = 0;
        return function() {return counter += 1;}
    }
)();

// return x,y axis for required chip_id
var get_chip_axis = function(chip_id) {
    var x_axis = 7.680;
    var y_axis = 147.940;
    if( typeof chip_id == "number") {
        // valid input, do something.
    }
    return {
        xAxis: x_axis,
        yAxis: y_axis
    };
};
