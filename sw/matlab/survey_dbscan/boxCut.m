%-------------------------------------------------------------------------------
  %
  %  Filename       : boxCut.m
  %  Author         : Huang Leilei
  %  Created        : 2020-10-26
  %  Description    : box cut algorithm
  %
%-------------------------------------------------------------------------------

function [idxGrp] = boxCut(datPnt)

%*** PARAMETER *****************************************************************
    IDX_RNG = 1;
    IDX_VEL = 2;
    IDX_ANG = 3;
    IDX_SNR = 4;


%*** MAIN BODY *****************************************************************
    [num, siz] = size(datPnt);
    idxGrp = zeros(num, 1);
    idxGrpCur = 0;
    for i = 1:num
        if idxGrp(i) == 0
            idxGrpCur = idxGrpCur + 1;
            idxGrp(i) = idxGrpCur;
            datRngI = datPnt(i,IDX_RNG);
            datVelI = datPnt(i,IDX_VEL);
            datAngI = datPnt(i,IDX_ANG);
            datRngIX = datRngI * cos(datAngI);
            datVelIX = datVelI * sin(datAngI);
            datRngIY = datRngI * cos(datAngI);
            datVelIY = datVelI * sin(datAngI);
            for j = i+1:num
                datRngJ = datPnt(j,IDX_RNG);
                datVelJ = datPnt(j,IDX_VEL);
                datAngJ = datPnt(j,IDX_ANG);
                datRngJX = datRngJ * cos(datAngI);
                datVelJX = datVelJ * sin(datAngI);
                datRngJY = datRngJ * cos(datAngI);
                datVelJY = datVelJ * sin(datAngI);
                if   abs(datRngIY - datRngJY) < 5                ...
                  && abs(datRngIX - datRngJX) < 2                ...
                  && abs(datVelIY - datVelJY) < 1.5              ...
                  && abs(datVelIX - datVelJX) < 1                ...
                  && abs(datAngI  - datAngJ ) < 20 / 180 * pi
                    idxGrp(j) = idxGrpCur;
                end
            end
        end
    end
end
