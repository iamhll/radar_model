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
    CUT = [5, 2, 1.5, 1, 20];
    [num, ~] = size(datPnt);
    [~, idxSort] = sort(datPnt(:,IDX_RNG),'descend');
    idxGrp = zeros(num, 1);
    idxGrpCur = 0;
    for iFor = 1:num
        i = idxSort(iFor);
        if idxGrp(i) == 0
            idxGrpCur = idxGrpCur + 1;
            idxGrp(i) = idxGrpCur;
            datRngI  = datPnt(i,IDX_RNG);
            datVelI  = datPnt(i,IDX_VEL);
            datAngI  = datPnt(i,IDX_ANG);
            datRngIX = datRngI * cos(datAngI);
            datVelIX = datVelI * sin(datAngI);
            datRngIY = datRngI * cos(datAngI);
            datVelIY = datVelI * sin(datAngI);
            for jFor = iFor+1:num
                j = idxSort(jFor);
                datRngJ  = datPnt(j,IDX_RNG);
                datVelJ  = datPnt(j,IDX_VEL);
                datAngJ  = datPnt(j,IDX_ANG);
                datRngJX = datRngJ * cos(datAngI);
                datVelJX = datVelJ * sin(datAngI);
                datRngJY = datRngJ * cos(datAngI);
                datVelJY = datVelJ * sin(datAngI);
                if   abs(datRngIY - datRngJY) <= CUT(1)    ...
                  && abs(datRngIX - datRngJX) <= CUT(2)    ...
                  && abs(datVelIY - datVelJY) <= CUT(3)    ...
                  && abs(datVelIX - datVelJX) <= CUT(4)    ...
                  && abs(datAngI  - datAngJ ) <= CUT(5) / 180 * pi
                    idxGrp(j) = idxGrpCur;
                    datRngI   = datRngJ ;
                    datVelI   = datVelJ ;
                    datAngI   = datAngJ ;
                    datRngIX  = datRngJX;
                    datVelIX  = datVelJX;
                    datRngIY  = datRngJY;
                    datVelIY  = datVelJY;
                end
            end
        end
    end
end
