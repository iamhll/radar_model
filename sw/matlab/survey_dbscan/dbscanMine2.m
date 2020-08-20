%-------------------------------------------------------------------------------
  %
  %  Filename       : dbscanMine2.m
  %  Author         : Huang Leilei
  %  Created        : 2020-08-17
  %  Description    : my dbscan
  %
%-------------------------------------------------------------------------------

function [idxGrpLst, idxKnlLst] = dbscanMine2(cost, epsilon, minpts)
    % declare spaces
    numDat    = size(cost, 1);
    idxGrpLst = zeros(numDat, 1);
    idxKnlLst = zeros(numDat, 1);
    idxTstLst = zeros(numDat, 1);

    % main loop
    idxGrpCur = 0;
    for idxDat = 1:numDat
        % if not visited
        if idxGrpLst(idxDat) == 0
            % get neighbour
            idxNeiLst = cost(idxDat,:) <= epsilon;

            % test minpts condition: pass
            if sum(idxNeiLst) >= minpts
                % mark it with a new group(cluster) id
                idxGrpCur = idxGrpCur + 1;
                idxGrpLst(idxDat) = idxGrpCur;

                % mark it as a kernal points
                idxKnlLst(idxDat) = 1;

                % expand this group (cluster)
                idxTstNum = 1;
                idxTstLst(idxTstNum) = idxDat;
                idxTstJ = 1;
                while idxTstJ <= idxTstNum
                    % get neighbour
                    idxNeiLst = cost(idxTstLst(idxTstJ),:) <= epsilon;

                    % loop each neighbour
                    for idxTstI = 1:numDat
                        % test if it is an unvisited neigbour: pass
                        if idxNeiLst(idxTstI) == 1 && idxGrpLst(idxTstI) == 0
                            % mark it with the current group(cluster) id
                            idxGrpLst(idxTstI) = idxGrpCur;

                            % test minpts condition: pass
                            if sum(cost(idxTstI,:)) >= minpts
                                % mark it as a kernal points
                                idxKnlLst(idxTstI) = 1;

                                % add it to test list
                                idxTstNum = idxTstNum + 1;
                                idxTstLst(idxTstNum) = idxTstI;
                            end
                        end
                    end

                    % udpate counter
                    idxTstJ = idxTstJ + 1;
                end

            % test minpts condition: fail
            else
                idxGrpLst(idxDat) = -1;
            end
        end
    end
end
