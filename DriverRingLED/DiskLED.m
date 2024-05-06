classdef DiskLED    
    properties (SetAccess = private)
        port
    end
    
    properties (Constant = true)
        HANDSHAKE     = 0
        LIGHTUPSINGLE = 1
        TURNOFFSINGLE = 2
        LIGHTUPRING   = 3
        TURNOFFRING   = 4
        LIGHTUPALL    = 5
        TURNOFFALL    = 6
        SETBRIGHTNESS = 7
    end

    
    methods
        function this = DiskLED(serialPort)
            this.port = serialport(serialPort, 115200); %baudRate modified based on the ardurnio
        end
        
        function init(this)
            fopen(this.port);
        end

        function stop(this)
            fclose(this.port);
        end
        
        function lightUpSingle(this, IDX, R, G, B)
            fprintf(this.port, '%c%c%c%c%c%c', [this.LIGHTUPSINGLE, IDX, R, G, B, 'x']);
        end

        function turnOffSingle(this, IDX)
            fprintf(this.port, '%c%c%c', [this.TURNOFFSINGLE, IDX, 'x']);
        end

        function lightUpRing(this, IDX, R, G, B)
            fprintf(this.port, '%c%c%c%c%c%c', [this.LIGHTUPRING, IDX, R, G, B, 'x']);
        end

        function turnOffRing(this, IDX)
            fprintf(this.port, '%c%c%c', [this.TURNOFFRING, IDX, 'x']);
        end

        function lightUpAll(this, R, G, B)
            fprintf(this.port, '%c%c%c%c%c', [this.LIGHTUPALL, R, G, B, 'x']);
        end

        function turnOffAll(this)
            fprintf(this.port, '%c%c', [this.TURNOFFALL, 'x']);
        end

        function setBrightness(this, LEVEL)
            fprintf(this.port, '%c%c%c', [this.SETBRIGHTNESS, LEVEL, 'x']);
        end
    end
    
end

