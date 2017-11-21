// workscript, a independent thread that runs motion controls.

WorkerScript.onMessage = function(backend, command) {
    backend.run_cmd(command)
    WorkerScript.sendMessage(0);
}
