; ModuleID = 'test/loopFus2.ll'
source_filename = "test/loopFus2.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@__const.loopFusionExample.b = private unnamed_addr constant [5 x i32] [i32 1, i32 2, i32 3, i32 4, i32 5], align 16

; Function Attrs: noinline nounwind uwtable
define dso_local void @loopFusionExample(i32 noundef %0) #0 {
  %2 = alloca [5 x i32], align 16
  %3 = alloca [5 x i32], align 16
  %4 = alloca [5 x i32], align 16
  %5 = alloca [5 x i32], align 16
  call void @llvm.memset.p0.i64(ptr align 16 %2, i8 0, i64 20, i1 false)
  call void @llvm.memcpy.p0.p0.i64(ptr align 16 %3, ptr align 16 @__const.loopFusionExample.b, i64 20, i1 false)
  call void @llvm.memset.p0.i64(ptr align 16 %4, i8 0, i64 20, i1 false)
  call void @llvm.memset.p0.i64(ptr align 16 %5, i8 0, i64 20, i1 false)
  br label %6

6:                                                ; preds = %15, %1
  %.01 = phi i32 [ 0, %1 ], [ %16, %15 ]
  %7 = icmp slt i32 %.01, %0
  br i1 %7, label %8, label %17

8:                                                ; preds = %6
  %9 = sext i32 %.01 to i64
  %10 = getelementptr inbounds [5 x i32], ptr %3, i64 0, i64 %9
  %11 = load i32, ptr %10, align 4
  %12 = add nsw i32 %11, 2
  %13 = sext i32 %.01 to i64
  %14 = getelementptr inbounds [5 x i32], ptr %2, i64 0, i64 %13
  store i32 %12, ptr %14, align 4
  br label %15

15:                                               ; preds = %8
  %16 = add nsw i32 %.01, 1
  br label %6, !llvm.loop !6

17:                                               ; preds = %6
  br label %18

18:                                               ; preds = %28, %17
  %.02 = phi i32 [ 0, %17 ], [ %29, %28 ]
  %19 = icmp slt i32 %.02, %0
  br i1 %19, label %20, label %30

20:                                               ; preds = %18
  %21 = add nsw i32 %.02, 3
  %22 = sext i32 %21 to i64
  %23 = getelementptr inbounds [5 x i32], ptr %3, i64 0, i64 %22
  %24 = load i32, ptr %23, align 4
  %25 = mul nsw i32 %24, 2
  %26 = sext i32 %.02 to i64
  %27 = getelementptr inbounds [5 x i32], ptr %4, i64 0, i64 %26
  store i32 %25, ptr %27, align 4
  br label %28

28:                                               ; preds = %20
  %29 = add nsw i32 %.02, 1
  br label %18, !llvm.loop !8

30:                                               ; preds = %18
  br label %31

31:                                               ; preds = %39, %30
  %.0 = phi i32 [ 0, %30 ], [ %40, %39 ]
  %32 = icmp slt i32 %.0, %0
  br i1 %32, label %33, label %41

33:                                               ; preds = %31
  %34 = sext i32 %.0 to i64
  %35 = getelementptr inbounds [5 x i32], ptr %3, i64 0, i64 %34
  %36 = load i32, ptr %35, align 4
  %37 = sext i32 %.0 to i64
  %38 = getelementptr inbounds [5 x i32], ptr %5, i64 0, i64 %37
  store i32 %36, ptr %38, align 4
  br label %39

39:                                               ; preds = %33
  %40 = add nsw i32 %.0, 1
  br label %31, !llvm.loop !9

41:                                               ; preds = %31
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #1

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #2

attributes #0 = { noinline nounwind uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 8, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 2}
!4 = !{i32 7, !"frame-pointer", i32 2}
!5 = !{!"clang version 19.1.7 (/home/runner/work/llvm-project/llvm-project/clang cd708029e0b2869e80abe31ddb175f7c35361f90)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
